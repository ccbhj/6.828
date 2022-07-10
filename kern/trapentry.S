/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>
# Name	Vector nr.	Type	Mnemonic	Error code?
# Divide-by-zero Error	0 (0x0)	Fault	#DE	No
# Debug	1 (0x1)	Fault/Trap	#DB	No
# Non-maskable Interrupt	2 (0x2)	Interrupt	-	No
# Breakpoint	3 (0x3)	Trap	#BP	No
# Overflow	4 (0x4)	Trap	#OF	No
# Bound Range Exceeded	5 (0x5)	Fault	#BR	No
# Invalid Opcode	6 (0x6)	Fault	#UD	No
# Device Not Available	7 (0x7)	Fault	#NM	No
# Double Fault	8 (0x8)	Abort	#DF	Yes (Zero)
# Coprocessor Segment Overrun	9 (0x9)	Fault	-	No
# Invalid TSS	10 (0xA)	Fault	#TS	Yes
# Segment Not Present	11 (0xB)	Fault	#NP	Yes
# Stack-Segment Fault	12 (0xC)	Fault	#SS	Yes
# General Protection Fault	13 (0xD)	Fault	#GP	Yes
# Page Fault	14 (0xE)	Fault	#PF	Yes
# Reserved	15 (0xF)	-	-	No
# x87 Floating-Point Exception	16 (0x10)	Fault	#MF	No
# Alignment Check	17 (0x11)	Fault	#AC	Yes
# Machine Check	18 (0x12)	Abort	#MC	No
# SIMD Floating-Point Exception	19 (0x13)	Fault	#XM/#XF	No
# Virtualization Exception	20 (0x14)	Fault	#VE	No
# Control Protection Exception	21 (0x15)	Fault	#CP	Yes
# Reserved	22-27 (0x16-0x1B)	-	-	No
# Hypervisor Injection Exception	28 (0x1C)	Fault	#HV	No
# VMM Communication Exception	29 (0x1D)	Fault	#VC	Yes
# Security Exception	30 (0x1E)	Fault	#SX	Yes
# Reserved	31 (0x1F)	-	-	No
# Triple Fault	-	-	-	No
# FPU Error Interrupt	IRQ 13	Interrupt	#FERR	No



###################################################################
# exceptions/interrupts
###################################################################

/* TRAPHANDLER defines a globally-visible function for handling a trap.
 * It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 *
 * You shouldnot call a TRAPHANDLER function from C, but you may
 * need to _declare_ one in C (for instance, to get a function pointer
 * during IDT setup).  You can declare the function with
 *   void NAME();
 * where NAME is the argument passed to TRAPHANDLER.
 */

#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for name */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesnot push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

.text

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */
trap_handlers:
	TRAPHANDLER_NOEC(trap_divide_zero, T_DIVIDE);
	TRAPHANDLER_NOEC(trap_debug, T_DEBUG);
	TRAPHANDLER_NOEC(trap_nmi, T_NMI);
	TRAPHANDLER_NOEC(trap_breakpoint, T_BRKPT);
	TRAPHANDLER_NOEC(trap_overflow, T_OFLOW);
	TRAPHANDLER_NOEC(trap_bounds_check, T_BOUND);
	TRAPHANDLER_NOEC(trap_illegal_op, T_ILLOP);
	TRAPHANDLER_NOEC(trap_device, T_DEVICE);
	TRAPHANDLER(trap_double_fault, T_DBLFLT);
	TRAPHANDLER(trap_tss, T_TSS);
	TRAPHANDLER(trap_segnp, T_SEGNP);
	TRAPHANDLER(trap_stack, T_STACK);
	TRAPHANDLER(trap_general_protect_fault, T_GPFLT);
	TRAPHANDLER(trap_page_fault, T_PGFLT);
	TRAPHANDLER_NOEC(trap_float_point_error, T_FPERR);
	TRAPHANDLER(trap_alignment, T_ALIGN);
	TRAPHANDLER_NOEC(trap_machine, T_MCHK);
	TRAPHANDLER(trap_smid_flaot_point_error, T_SIMDERR);

	TRAPHANDLER_NOEC(trap_syscall, T_SYSCALL);


/*
 * Lab 3: Your code here for _alltraps
 */
_alltraps:
	pushl %ds       # ds
	pushl %es       # es
	pushal          # tf_regs

	movw $GD_KD, %ax
	movw %ax, %ds
	movw %ax, %es

	pushl %esp      # push the esp as (struct Tramframe*)

	call trap
