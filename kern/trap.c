#include "inc/trap.h"
#include "inc/error.h"
#include "inc/memlayout.h"
#include "inc/stdio.h"
#include <inc/mmu.h>
#include <inc/x86.h>
#include <inc/assert.h>

#include <kern/pmap.h>
#include <kern/trap.h>
#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/env.h>
#include <kern/syscall.h>
#include "kdebug.h"

static struct Taskstate ts;

/* For debugging, so print_trapframe can distinguish between printing
 * a saved trapframe and printing the current trapframe and print some
 * additional information in the latter case.
 */
static struct Trapframe *last_tf;

/* Interrupt descriptor table.  (Must be built at run time because
 * shifted function addresses can't be represented in relocation records.)
 */
struct Gatedesc idt[256] = { { 0 } };
struct Pseudodesc idt_pd = {
	sizeof(idt) - 1, (uint32_t) idt
};


static const char *trapname(int trapno)
{
	static const char * const excnames[] = {
		"Divide error",
		"Debug",
		"Non-Maskable Interrupt",
		"Breakpoint",
		"Overflow",
		"BOUND Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection",
		"Page Fault",
		"(unknown trap)",
		"x87 FPU Floating-Point Error",
		"Alignment Check",
		"Machine-Check",
		"SIMD Floating-Point Exception"
	};

	if (trapno < ARRAY_SIZE(excnames))
		return excnames[trapno];
	if (trapno == T_SYSCALL)
		return "System call";
	return "(unknown trap)";
}

// selector must bge GD_KT, or you will get page fault
#define TRAP_INIT_GATE(fn, num) \
	SETGATE(idt[num], 1, GD_KT, (unsigned)(fn), 0)
	
#define TRAP_INIT_GATE_USER(fn, num) \
	SETGATE(idt[num], 1, GD_KT, (unsigned)(fn), 3)

void
trap_init(void)
{
	extern struct Segdesc gdt[];

	// LAB 3: Your code here.
	TRAP_INIT_GATE(trap_divide_zero, T_DIVIDE);
	TRAP_INIT_GATE_USER(trap_debug, T_DEBUG);
	TRAP_INIT_GATE(trap_nmi, T_NMI);
	TRAP_INIT_GATE_USER(trap_breakpoint, T_BRKPT);
	TRAP_INIT_GATE(trap_overflow, T_OFLOW);
	TRAP_INIT_GATE(trap_bounds_check, T_BOUND);
	TRAP_INIT_GATE(trap_illegal_op, T_ILLOP);
	TRAP_INIT_GATE(trap_device, T_DEVICE);
	TRAP_INIT_GATE(trap_double_fault, T_DBLFLT);
	TRAP_INIT_GATE(trap_tss, T_TSS);
	TRAP_INIT_GATE(trap_segnp, T_SEGNP);
	TRAP_INIT_GATE(trap_stack, T_STACK);
	TRAP_INIT_GATE(trap_general_protect_fault, T_GPFLT);
	TRAP_INIT_GATE(trap_page_fault, T_PGFLT);
	TRAP_INIT_GATE(trap_float_point_error, T_FPERR);
	TRAP_INIT_GATE(trap_alignment, T_ALIGN);
	TRAP_INIT_GATE(trap_machine, T_MCHK);
	TRAP_INIT_GATE(trap_smid_flaot_point_error, T_SIMDERR);

	TRAP_INIT_GATE_USER(trap_syscall, T_SYSCALL);


	// Per-CPU setup 
	trap_init_percpu();
}

// Initialize and load the per-CPU TSS and IDT
void
trap_init_percpu(void)
{
	// Setup a TSS so that we get the right stack
	// when we trap to the kernel.
	ts.ts_esp0 = KSTACKTOP;
	ts.ts_ss0 = GD_KD;
	ts.ts_iomb = sizeof(struct Taskstate);

	// Initialize the TSS slot of the gdt.
	gdt[GD_TSS0 >> 3] = SEG16(STS_T32A, (uint32_t) (&ts),
					sizeof(struct Taskstate) - 1, 0);
	gdt[GD_TSS0 >> 3].sd_s = 0;

	// Load the TSS selector (like other segment selectors, the
	// bottom three bits are special; we leave them 0)
	ltr(GD_TSS0);

	// Load the IDT
	lidt(&idt_pd);
}

void
print_trapframe(struct Trapframe *tf)
{
	cprintf("TRAP frame at %p, last_tf %p\n", tf, last_tf);
	print_regs(&tf->tf_regs);
	cprintf("  es   0x----%04x\n", tf->tf_es);
	cprintf("  ds   0x----%04x\n", tf->tf_ds);
	cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
	// If this trap was a page fault that just happened
	// (so %cr2 is meaningful), print the faulting linear address.
	if (tf == last_tf && tf->tf_trapno == T_PGFLT)
		cprintf("  cr2  0x%08x\n", rcr2());
	cprintf("  err  0x%08x", tf->tf_err);
	// For page faults, print decoded fault error code:
	// U/K=fault occurred in user/kernel mode
	// W/R=a write/read caused the fault
	// PR=a protection violation caused the fault (NP=page not present).
	if (tf->tf_trapno == T_PGFLT)
		cprintf(" [%s, %s, %s]\n",
			tf->tf_err & 4 ? "user" : "kernel",
			tf->tf_err & 2 ? "write" : "read",
			tf->tf_err & 1 ? "protection" : "not-present");
	else
		cprintf("\n");
	cprintf("  eip  0x%08x\n", tf->tf_eip);
	cprintf("  cs   0x----%04x\n", tf->tf_cs);
	cprintf("  flag 0x%08x\n", tf->tf_eflags);
	// trap from kernel mode
	if ((tf->tf_cs & 3) != 0) {
		cprintf("  esp  0x%08x\n", tf->tf_esp);
		cprintf("  ss   0x----%04x\n", tf->tf_ss);
	}
}

void
print_regs(struct PushRegs *regs)
{
	cprintf("  edi  0x%08x\n", regs->reg_edi);
	cprintf("  esi  0x%08x\n", regs->reg_esi);
	cprintf("  ebp  0x%08x\n", regs->reg_ebp);
	cprintf("  oesp 0x%08x\n", regs->reg_oesp);
	cprintf("  ebx  0x%08x\n", regs->reg_ebx);
	cprintf("  edx  0x%08x\n", regs->reg_edx);
	cprintf("  ecx  0x%08x\n", regs->reg_ecx);
	cprintf("  eax  0x%08x\n", regs->reg_eax);
}

static void
trap_dispatch(struct Trapframe *tf)
{
	// Handle processor exceptions.
	// LAB 3: Your code here.
	switch (tf->tf_trapno) {
		case T_PGFLT  : 
			page_fault_handler(tf);
			return;
		case T_BRKPT  : 
			trap_breakpoint_handler(tf);
			break;
		case T_SYSCALL:
			trap_syscall_handler(tf);
			break;
		case T_DIVIDE : 
		case T_DEBUG  : 
		case T_NMI    : 
		case T_OFLOW  : 
		case T_BOUND  : 
		case T_ILLOP  : 
		case T_DEVICE : 
		case T_DBLFLT : 
		case T_TSS    : 
		case T_SEGNP  : 
		case T_STACK  : 
		case T_GPFLT  : 
		case T_FPERR  : 
		case T_ALIGN  : 
		case T_MCHK   : 
		case T_SIMDERR: 
			trap_destruction_handler(tf);
			return;
	}


	// Unexpected trap: The user process or the kernel has a bug.
	print_trapframe(tf);
	if (tf->tf_cs == GD_KT)
		panic("unhandled trap in kernel");
	else {
		env_destroy(curenv);
		return;
	}
}

void
trap(struct Trapframe *tf)
{
	// The environment may have set DF and some versions
	// of GCC rely on DF being clear
	asm volatile("cld" ::: "cc");

	// Check that interrupts are disabled.  If this assertion
	// fails, DO NOT be tempted to fix it by inserting a "cli" in
	// the interrupt path.
	assert(!(read_eflags() & FL_IF));

	cprintf("Incoming TRAP frame at %p\n", tf);
	DEBUG("TRAP num = %d\n", tf->tf_trapno);

	if ((tf->tf_cs & 3) == 3) {
		// Trapped from user mode.
		assert(curenv);

		// Copy trap frame (which is currently on the stack)
		// into 'curenv->env_tf', so that running the environment
		// will restart at the trap point.
		curenv->env_tf = *tf;
		// The trapframe on the stack should be ignored from here on.
		tf = &curenv->env_tf;
	}

	// Record that tf is the last real trapframe so
	// print_trapframe can print some additional information.
	last_tf = tf;

	// Dispatch based on what type of trap occurred
	trap_dispatch(tf);

	// Return to the current environment, which should be running.
	assert(curenv && curenv->env_status == ENV_RUNNING);
	env_run(curenv);
}


void
page_fault_handler(struct Trapframe *tf)
{
	uint32_t fault_va;

	// Read processor's CR2 register to find the faulting address
	fault_va = rcr2();

	// Handle kernel-mode page faults.
	if ((tf->tf_cs & 3) != 3) {
		print_trapframe(tf);
		panic("[%08x] kernel page fault va %08x ip %08x\n",
			curenv->env_id, fault_va, tf->tf_eip);
	}

	// LAB 3: Your code here.

	// We've already handled kernel-mode exceptions, so if we get here,
	// the page fault happened in user mode.

	// Destroy the environment that caused the fault.
	cprintf("[%08x] user fault va %08x ip %08x\n",
		curenv->env_id, fault_va, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);
}

void
trap_breakpoint_handler(struct Trapframe *tf)
{
	while (1) {
		monitor(tf);
	}
}

void
trap_destruction_handler(struct Trapframe *tf)
{
	print_trapframe(tf);
	env_destroy(curenv);
}

void
trap_syscall_handler(struct Trapframe *tf)
{
	uint32_t ret;
	ret = syscall(tf->tf_regs.reg_eax, tf->tf_regs.reg_edx, tf->tf_regs.reg_ecx, tf->tf_regs.reg_ebx, tf->tf_regs.reg_edi, tf->tf_regs.reg_esi);
	tf->tf_regs.reg_eax = ret;
	if (ret == -E_INVAL)
		env_destroy(curenv);
	else
		env_run(curenv);
}
