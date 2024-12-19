// User-level page fault handler support.
// Rather than register the C page fault handler directly with the
// kernel as the page fault handler, we register the assembly language
// wrapper in pfentry.S, which in turns calls the registered C
// function.

#include "inc/env.h"
#include "inc/log.h"
#include "inc/memlayout.h"
#include "inc/mmu.h"
#include "inc/string.h"
#include "inc/syscall.h"
#include "inc/trap.h"
#include "inc/types.h"
#include <inc/lib.h>


// Assembly language pgfault entrypoint defined in lib/pfentry.S.
extern void _pgfault_upcall(void);

// Pointer to currently installed C-language pgfault handler.
void (*_pgfault_handler)(struct UTrapframe *utf);

//
// Set the page fault handler function.
// If there isn't one yet, _pgfault_handler will be 0.
// The first time we register a handler, we need to
// allocate an exception stack (one page of memory with its top
// at UXSTACKTOP), and tell the kernel to call the assembly-language
// _pgfault_upcall routine when a page fault occurs.
//
void
set_pgfault_handler(void (*handler)(struct UTrapframe *utf))
{
	int r;

	if (_pgfault_handler == 0) {
		// First time through!
		// LAB 4: Your code here.
		if (sys_page_alloc(0, (void*)(UXSTACKTOP - PGSIZE), PTE_P | PTE_U | PTE_W | PTE_AVAIL) < 0) 
			panic("fault to allocate UXSTACKTOP");
	}

	// Save handler pointer for assembly to call.
	_pgfault_handler = handler;
	sys_env_set_pgfault_upcall(0, _pgfault_upcall);
}

int set_pgfault_handler_for_child(envid_t id, void (*handler)(struct UTrapframe *utf)) 
{
	int ret;
	void *va;
	uintptr_t off;

	// va = (void*)ROUNDDOWN((uintptr_t)(&_pgfault_upcall), PGSIZE);
	// DEBUG("mapping _pgfault_upcall at 0x%x\n", va);

	// if ((ret = sys_page_map(0, va, id, va, PTE_P | PTE_U))) {
	// 	ERR("fail to map page of va %p for _pgfault_upcall\n", va);
	// 	return ret;
	// }

	va = (void*)ROUNDDOWN((uintptr_t)(&_pgfault_handler), PGSIZE);
	DEBUG("replace _pgfault_handler at 0x%x\n", va);
	if ((ret = sys_page_alloc(id, va, PTE_P | PTE_U | PTE_W | PTE_AVAIL))) {
		ERR("fail to alloc page for _pgfault_handler at va %p: %e\n", va, ret);
		return ret;
	}

	if ((ret = sys_page_map(id, va, 0, UTEMP, PTE_U | PTE_W | PTE_P | PTE_AVAIL))) {
		ERR("fail to map page of va %p at UTEMP: %e\n", va, ret);
		return ret;
	}
	memcpy(UTEMP, va, PGSIZE);

	off = (uintptr_t)(&_pgfault_handler) - (uintptr_t)va;

	void (**p)(struct UTrapframe*) = (UTEMP + off);
	DEBUG("new pgfault handler at TEMP pointer: %p, offset=0x%x\n", p, off);
	*p = handler;

	if ((ret = sys_page_unmap(0, UTEMP))) {
		ERR("fail to unmap page of va %p at UTEMP: %e", va, ret);
		return ret;
	}

	if ((ret = sys_env_set_pgfault_upcall(id, _pgfault_upcall))) {
		ERR("fail to set pgfault_upcall for env %d: %e", id, ret);
		return ret;
	}

	return 0;

bad:
	sys_page_unmap(id, va);
	return ret;
}
