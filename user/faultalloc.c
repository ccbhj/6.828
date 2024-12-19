// test user-level fault handler -- alloc pages to fix faults

#include "inc/stdio.h"
#include <inc/lib.h>

void
handler(struct UTrapframe *utf)
{
	int r;
	void *addr = (void*)utf->utf_fault_va;

	cprintf("fault %x\n", addr);
	cprintf("esp %x\n", utf->utf_esp);
	cprintf("eip %x\n", utf->utf_eip);
	cprintf("utf %p\n", utf);
	if ((r = sys_page_alloc(0, ROUNDDOWN(addr, PGSIZE),
				PTE_P|PTE_U|PTE_W)) < 0)
		panic("allocating at %x in page fault handler: %e", addr, r);
	snprintf((char*) addr, 100, "this string was faulted in at %x", addr);
	cprintf("exit handler\n");
}

void
umain(int argc, char **argv)
{
	set_pgfault_handler(handler);
	cprintf("%s\n", (char*)0xDeadBeef);
	cprintf("%s\n", (char*)0xCafeBffe);
	cprintf("done!!!\n");
}
