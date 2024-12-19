/* See COPYRIGHT for copyright information. */

#ifndef JOS_KERN_TRAP_H
#define JOS_KERN_TRAP_H
#ifndef JOS_KERNEL
# error "This is a JOS kernel header; user programs should not #include it"
#endif

#include <inc/trap.h>
#include <inc/mmu.h>

#define DECLARE_TRAP_FUNC(name) \
	void(name)(struct Trapframe*)

/* The kernel's interrupt descriptor table */
extern struct Gatedesc idt[];
extern struct Pseudodesc idt_pd;

void trap_init(void);
void trap_init_percpu(void);
void print_regs(struct PushRegs *regs);
void print_trapframe(struct Trapframe *tf);
void page_fault_handler(struct Trapframe *);
void backtrace(struct Trapframe *);

void trap_destruction_handler(struct Trapframe *tf);
void trap_breakpoint_handler(struct Trapframe *tf);
void trap_syscall_handler(struct Trapframe *tf);
void irq_timer_handler(struct Trapframe *tf);

DECLARE_TRAP_FUNC(trap_divide_zero);
DECLARE_TRAP_FUNC(trap_debug);
DECLARE_TRAP_FUNC(trap_nmi);
DECLARE_TRAP_FUNC(trap_breakpoint);
DECLARE_TRAP_FUNC(trap_overflow);
DECLARE_TRAP_FUNC(trap_bounds_check);
DECLARE_TRAP_FUNC(trap_illegal_op);
DECLARE_TRAP_FUNC(trap_device);
DECLARE_TRAP_FUNC(trap_double_fault);
DECLARE_TRAP_FUNC(trap_tss);
DECLARE_TRAP_FUNC(trap_segnp);
DECLARE_TRAP_FUNC(trap_stack);
DECLARE_TRAP_FUNC(trap_general_protect_fault);
DECLARE_TRAP_FUNC(trap_page_fault);
DECLARE_TRAP_FUNC(trap_float_point_error);
DECLARE_TRAP_FUNC(trap_alignment);
DECLARE_TRAP_FUNC(trap_machine);
DECLARE_TRAP_FUNC(trap_smid_flaot_point_error);
DECLARE_TRAP_FUNC(trap_syscall);

DECLARE_TRAP_FUNC(irq_timer);
DECLARE_TRAP_FUNC(irq_kbd);
DECLARE_TRAP_FUNC(irq_serial);
DECLARE_TRAP_FUNC(irq_spurious);
DECLARE_TRAP_FUNC(irq_ide);
#endif /* JOS_KERN_TRAP_H */
