// implement fork from user space

#include "inc/assert.h"
#include "inc/env.h"
#include "inc/log.h"
#include "inc/memlayout.h"
#include "inc/mmu.h"
#include "inc/stdio.h"
#include "inc/types.h"
#include "inc/x86.h"
#include "kern/kdebug.h"
#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int r;
	int pgnum;
	uint32_t pgstart;
	void *temp;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.
	INFO("env_id=0x%x, pgfault_va=%p, eip=0x%x\n",
			sys_getenvid(), addr, utf->utf_eip);
	pgnum = (uint32_t)addr >> PGSHIFT;
	pgstart = (uint32_t)ROUNDDOWN((uint32_t)addr, PGSIZE);
	temp = (void*)(uintptr_t)PFTEMP;
	DEBUG("env_id=0x%x, pgfault_va=%p, duppage at 0x%x, PFTEMP=%p\n",
			sys_getenvid(), addr, pgstart, temp);
	if (!(err & FEC_WR) && !(uvpt[pgnum] & PTE_COW)) {
		panic("page fault va not COW, va=0x%x, pte=0x%x\n", utf->utf_fault_va, uvpt[pgnum]);
	}

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.
	//
	// LAB 4: Your code here.
	if ((r = sys_page_alloc(0, temp, PTE_P | PTE_W | PTE_U))) 
		panic("fail to allocate page at PFTEMP: 0x%x\n: %e", PFTEMP, r);

	memcpy(temp, (void*)pgstart, PGSIZE);
	if ((r = sys_page_map(0, temp, 0, (void*)pgstart, PTE_P | PTE_U | PTE_W))) {
		panic("fail to map PFTEMP to pgstart 0x%x: %e\n", pgstart, r);
	}
	if ((r = sys_page_unmap(0, temp))) {
		ERR("fail to unmap PFTEMP 0x%x: %e\n", PFTEMP, r);
	}
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
static int
duppage(envid_t envid, unsigned pn)
{
	int r;
	pte_t pte;
	pde_t pde;
	int cow;
	void *va;

	cow = 0;
	if (pn > (NPDENTRIES * NPTENTRIES)) 
		panic("pn %d out of range\n", pn);
	pde = uvpd[pn >> 10];
	if (pn > NPTENTRIES * NPDENTRIES) 
		panic("pn %d out of range of uvpt\n", pn);
	pte = uvpt[pn];
	if (!(pte & PTE_P)) 
		return 0;
	va = (void*)(uintptr_t)(pn << PGSHIFT);

	if ((pte & PTE_W) || ( pte &PTE_COW))
		cow = PTE_COW;

	if ((r = sys_page_map(0, va, envid, va, PTE_P | PTE_U | cow))) {
		ERR("fail to map page, pn=%d, pte=0x%x, va=%p, dstenv=%d, cow=%d, err=%e\n", pn, pte, va, envid, cow != 0, r);
		return r;
	}

	if (cow && !(pte & PTE_W) &&
			(r = sys_page_map(0, va, 0, va, (pte & 0xFFF) | PTE_COW))) {
			ERR("fail to mark origin page COW, pn=%d, va=%p, err=%e\n", pn, va, r);
			return r;
		}

	return 0;
}

// copymap copy content of the page at addr and map it with perm
// return 0 on success
int copymap(envid_t id, void *addr, int perm) 
{
	void* pg;
	int ret;

	pg = (void*)ROUNDDOWN((uintptr_t)addr, PGSIZE);
	INFO("copy page from 0x%x\n", pg);
	if ((ret = sys_page_alloc(id, (void*)pg, perm))) {
		ERR("[%04x]fail to allocate page from 0x%x for env_id %04x: %e\n", thisenv->env_id, pg, id, ret);
		return ret;
	}

	if ((ret = sys_page_map(id, pg, 0, (void*)PFTEMP, PTE_P | PTE_U | PTE_W))) {
		ERR("[%04x]fail to map page from 0x%x for env_id %04x: %e\n", thisenv->env_id, pg, id, ret);
		return ret;
	}

	memcpy((void*)PFTEMP, pg, PGSIZE);
	if ((ret = sys_page_unmap(0, PFTEMP))) {
		ERR("[%04x]fail to unmap page from 0x%x for env_id %04x: %e\n", thisenv->env_id, PFTEMP, ret);
		return ret;
	}

	return 0;
bad:
	if ((ret = sys_page_unmap(id, (void*)pg))) {
		ERR("[%04x]fail to unmap page from 0x%x for env_id %04x: %e\n", thisenv->env_id, pg, id, ret);
		return ret;
	}
}

//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	// LAB 4: Your code here.
	uint32_t curstack; // to mark the stack
	envid_t cid, pid;
	int ret;
	unsigned pn, endpn;
	pte_t pte;

	pid = thisenv->env_id;

	if ((cid = sys_exofork()) < 0) {
		ERR("sys_exofork error: %e\n", cid);
		return cid;
	}
	
	// for the child env
	if (cid == 0) {
		thisenv = &envs[ENVX(sys_getenvid())];
		return 0;
	}

	DEBUG("new child env id %04x\n", cid);

	pn = 0;
	do {
		if ((pn << PGSHIFT) >= UTOP)
			break;
		if (!(uvpd[pn >> 10] & PTE_P)) {
			pn += NPDENTRIES -1;
			continue;
		}
		pte = uvpt[pn];
		if (((pte & PTE_P) && (ret = duppage(cid, pn)))) 
			goto bad;
	} while (++pn < (NPDENTRIES * NPTENTRIES));

	// copy the current stack
	if ((ret = copymap(cid, &curstack, PTE_P | PTE_U | PTE_W))) {
		panic("[%04x]fail to copy stack for cid %04x: %e", thisenv->env_id, cid, ret);
		goto bad;
	}

	// allocate a new page for UXSTACKTOP for the child env
	if ((ret = sys_page_alloc(cid, (void*)(UXSTACKTOP - PGSIZE), PTE_P | PTE_U | PTE_W))) {
		panic("fail to allocate a new page for child's UXSTACK, cid=%d, err=%e\n", cid, ret);
		goto bad;
	}

	if ((ret = set_pgfault_handler_for_child(cid, pgfault))) {
		panic("fail to set pgfault handler for child %d: %e", cid, ret);
		goto bad;
	}
	if ((ret = sys_env_set_status(cid, ENV_RUNNABLE)) < 0) {
		panic("fail to run the child env: %e", ret);
		goto bad;
	}
	return cid;

bad:
	sys_env_destroy(cid);
	panic("fail to fork: %e\n", ret);
	return ret;
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
