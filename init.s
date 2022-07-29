	.file	"init.c"
	.stabs	"kern/init.c",100,0,2,.Ltext0
	.text
.Ltext0:
	.stabs	"gcc2_compiled.",60,0,0,0
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"entering test_backtrace %d\n"
.LC1:
	.string	"leaving test_backtrace %d\n"
	.text
	.p2align 4
	.stabs	"test_backtrace:F(0,1)=(0,1)",36,0,0,test_backtrace
	.stabs	"void:t(0,1)",128,0,0,0
	.stabs	"x:P(0,2)=r(0,2);-2147483648;2147483647;",64,0,0,6
	.stabs	"int:t(0,2)",128,0,0,0
	.globl	test_backtrace
	.type	test_backtrace, @function
test_backtrace:
	.stabn	68,0,17,.LM0-.LFBB1
.LM0:
.LFBB1:
.LFB33:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	.stabn	68,0,18,.LM1-.LFBB1
.LM1:
	movl	%edi, %esi
	xorl	%eax, %eax
	.stabn	68,0,17,.LM2-.LFBB1
.LM2:
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	.stabn	68,0,18,.LM3-.LFBB1
.LM3:
	leaq	.LC0(%rip), %r12
	.stabn	68,0,17,.LM4-.LFBB1
.LM4:
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movl	%edi, %ebp
	.stabn	68,0,18,.LM5-.LFBB1
.LM5:
	movq	%r12, %rdi
	.stabn	68,0,17,.LM6-.LFBB1
.LM6:
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	.stabn	68,0,18,.LM7-.LFBB1
.LM7:
	call	cprintf@PLT
	.stabn	68,0,19,.LM8-.LFBB1
.LM8:
	testl	%ebp, %ebp
	jle	.L2
	.stabn	68,0,20,.LM9-.LFBB1
.LM9:
	leal	-1(%rbp), %r13d
.LBB12:
.LBB13:
	.stabn	68,0,18,.LM10-.LFBB1
.LM10:
	xorl	%eax, %eax
	movq	%r12, %rdi
	movl	%r13d, %esi
	call	cprintf@PLT
	.stabn	68,0,19,.LM11-.LFBB1
.LM11:
	testl	%r13d, %r13d
	je	.L3
	.stabn	68,0,20,.LM12-.LFBB1
.LM12:
	leal	-2(%rbp), %r14d
.LBB14:
.LBB15:
	.stabn	68,0,18,.LM13-.LFBB1
.LM13:
	xorl	%eax, %eax
	movq	%r12, %rdi
	movl	%r14d, %esi
	call	cprintf@PLT
	.stabn	68,0,19,.LM14-.LFBB1
.LM14:
	testl	%r14d, %r14d
	je	.L4
	.stabn	68,0,20,.LM15-.LFBB1
.LM15:
	leal	-3(%rbp), %r15d
.LBB16:
.LBB17:
	.stabn	68,0,18,.LM16-.LFBB1
.LM16:
	xorl	%eax, %eax
	movq	%r12, %rdi
	movl	%r15d, %esi
	call	cprintf@PLT
	.stabn	68,0,19,.LM17-.LFBB1
.LM17:
	testl	%r15d, %r15d
	je	.L5
	.stabn	68,0,20,.LM18-.LFBB1
.LM18:
	leal	-4(%rbp), %ebx
.LBB18:
.LBB19:
	.stabn	68,0,18,.LM19-.LFBB1
.LM19:
	xorl	%eax, %eax
	movq	%r12, %rdi
	movl	%ebx, %esi
	call	cprintf@PLT
	.stabn	68,0,19,.LM20-.LFBB1
.LM20:
	testl	%ebx, %ebx
	je	.L6
	.stabn	68,0,20,.LM21-.LFBB1
.LM21:
	leal	-5(%rbp), %edi
	call	test_backtrace
.L7:
	.stabn	68,0,23,.LM22-.LFBB1
.LM22:
	leaq	.LC1(%rip), %r12
	movl	%ebx, %esi
	xorl	%eax, %eax
	movq	%r12, %rdi
	call	cprintf@PLT
.L8:
.LBE19:
.LBE18:
	movl	%r15d, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	cprintf@PLT
.L9:
.LBE17:
.LBE16:
	movl	%r14d, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	cprintf@PLT
	.stabn	68,0,24,.LM23-.LFBB1
.LM23:
	jmp	.L10
	.p2align 4,,10
	.p2align 3
.L3:
.LBE15:
.LBE14:
	.stabn	68,0,22,.LM24-.LFBB1
.LM24:
	xorl	%edx, %edx
	xorl	%esi, %esi
	leaq	.LC1(%rip), %r12
	xorl	%edi, %edi
	call	mon_backtrace@PLT
.L10:
	.stabn	68,0,23,.LM25-.LFBB1
.LM25:
	movl	%r13d, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	cprintf@PLT
.L11:
.LBE13:
.LBE12:
	.stabn	68,0,24,.LM26-.LFBB1
.LM26:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	.stabn	68,0,23,.LM27-.LFBB1
.LM27:
	movl	%ebp, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	.stabn	68,0,24,.LM28-.LFBB1
.LM28:
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	.stabn	68,0,23,.LM29-.LFBB1
.LM29:
	jmp	cprintf@PLT
	.p2align 4,,10
	.p2align 3
.L2:
	.cfi_restore_state
	.stabn	68,0,22,.LM30-.LFBB1
.LM30:
	xorl	%edx, %edx
	xorl	%esi, %esi
	leaq	.LC1(%rip), %r12
	xorl	%edi, %edi
	call	mon_backtrace@PLT
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L5:
.LBB29:
.LBB28:
.LBB27:
.LBB26:
.LBB24:
.LBB22:
	xorl	%edx, %edx
	xorl	%esi, %esi
	leaq	.LC1(%rip), %r12
	xorl	%edi, %edi
	call	mon_backtrace@PLT
	jmp	.L8
	.p2align 4,,10
	.p2align 3
.L4:
.LBE22:
.LBE24:
	xorl	%edx, %edx
	xorl	%esi, %esi
	leaq	.LC1(%rip), %r12
	xorl	%edi, %edi
	call	mon_backtrace@PLT
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L6:
.LBB25:
.LBB23:
.LBB21:
.LBB20:
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	mon_backtrace@PLT
	jmp	.L7
.LBE20:
.LBE21:
.LBE23:
.LBE25:
.LBE26:
.LBE27:
.LBE28:
.LBE29:
	.cfi_endproc
.LFE33:
	.size	test_backtrace, .-test_backtrace
.Lscope1:
	.section	.rodata.str1.1
.LC2:
	.string	"6828 decimal is %o octal!\n"
	.text
	.p2align 4
	.stabs	"i386_init:F(0,1)",36,0,0,i386_init
	.globl	i386_init
	.type	i386_init, @function
i386_init:
	.stabn	68,0,28,.LM31-.LFBB2
.LM31:
.LFBB2:
.LFB34:
	.cfi_startproc
	.stabn	68,0,34,.LM32-.LFBB2
.LM32:
	leaq	edata(%rip), %rdi
	.stabn	68,0,34,.LM33-.LFBB2
.LM33:
	leaq	end(%rip), %rdx
	.stabn	68,0,28,.LM34-.LFBB2
.LM34:
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	.stabn	68,0,34,.LM35-.LFBB2
.LM35:
	xorl	%esi, %esi
	subl	%edi, %edx
	call	memset@PLT
	.stabn	68,0,38,.LM36-.LFBB2
.LM36:
	call	cons_init@PLT
	.stabn	68,0,40,.LM37-.LFBB2
.LM37:
	leaq	.LC2(%rip), %rdi
	movl	$6828, %esi
	xorl	%eax, %eax
	call	cprintf@PLT
	.stabn	68,0,44,.LM38-.LFBB2
.LM38:
	movl	$5, %edi
	call	test_backtrace
	.p2align 4,,10
	.p2align 3
.L15:
	.stabn	68,0,48,.LM39-.LFBB2
.LM39:
	xorl	%edi, %edi
	call	monitor@PLT
	jmp	.L15
	.cfi_endproc
.LFE34:
	.size	i386_init, .-i386_init
.Lscope2:
	.p2align 4
	.stabs	"my_test:F(0,1)",36,0,0,my_test
	.stabs	"a:P(0,3)=(0,4)=r(0,4);0;4294967295;",64,0,0,5
	.stabs	"b:P(0,3)",64,0,0,4
	.stabs	"c:P(0,3)",64,0,0,1
	.stabs	"uint32_t:t(0,3)",128,0,0,0
	.stabs	"unsigned int:t(0,4)",128,0,0,0
	.globl	my_test
	.type	my_test, @function
my_test:
	.stabn	68,0,53,.LM40-.LFBB3
.LM40:
.LFBB3:
.LFB35:
	.cfi_startproc
	.stabn	68,0,54,.LM41-.LFBB3
.LM41:
	ret
	.cfi_endproc
.LFE35:
	.size	my_test, .-my_test
.Lscope3:
	.section	.rodata.str1.1
.LC3:
	.string	"kernel panic at %s:%d: "
.LC4:
	.string	"\n"
	.text
	.p2align 4
	.stabs	"_panic:F(0,1)",36,0,0,_panic
	.stabs	"file:P(0,5)=*(0,6)=r(0,6);0;127;",64,0,0,5
	.stabs	"line:P(0,2)",64,0,0,4
	.stabs	"fmt:P(0,5)",64,0,0,6
	.stabs	"char:t(0,6)",128,0,0,0
	.globl	_panic
	.type	_panic, @function
_panic:
	.stabn	68,0,69,.LM42-.LFBB4
.LM42:
.LFBB4:
.LFB36:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdx, %rbp
	subq	$208, %rsp
	.cfi_def_cfa_offset 224
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	testb	%al, %al
	je	.L19
	movaps	%xmm0, 80(%rsp)
	movaps	%xmm1, 96(%rsp)
	movaps	%xmm2, 112(%rsp)
	movaps	%xmm3, 128(%rsp)
	movaps	%xmm4, 144(%rsp)
	movaps	%xmm5, 160(%rsp)
	movaps	%xmm6, 176(%rsp)
	movaps	%xmm7, 192(%rsp)
.L19:
	.stabn	68,0,69,.LM43-.LFBB4
.LM43:
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	.stabn	68,0,72,.LM44-.LFBB4
.LM44:
	cmpq	$0, panicstr(%rip)
	je	.L24
	.p2align 4,,10
	.p2align 3
.L21:
	.stabn	68,0,88,.LM45-.LFBB4
.LM45:
	xorl	%edi, %edi
	call	monitor@PLT
	jmp	.L21
.L24:
	.stabn	68,0,74,.LM46-.LFBB4
.LM46:
	movq	%rbp, panicstr(%rip)
	.stabn	68,0,77,.LM47-.LFBB4
.LM47:
#APP
# 77 "kern/init.c" 1
	cli; cld
# 0 "" 2
	.stabn	68,0,79,.LM48-.LFBB4
.LM48:
#NO_APP
	leaq	224(%rsp), %rax
	.stabn	68,0,80,.LM49-.LFBB4
.LM49:
	movl	%esi, %edx
	movq	%rdi, %rsi
	.stabn	68,0,79,.LM50-.LFBB4
.LM50:
	movl	$24, (%rsp)
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	.stabn	68,0,80,.LM51-.LFBB4
.LM51:
	leaq	.LC3(%rip), %rdi
	.stabn	68,0,79,.LM52-.LFBB4
.LM52:
	movq	%rax, 16(%rsp)
	.stabn	68,0,80,.LM53-.LFBB4
.LM53:
	xorl	%eax, %eax
	.stabn	68,0,79,.LM54-.LFBB4
.LM54:
	movl	$48, 4(%rsp)
	.stabn	68,0,80,.LM55-.LFBB4
.LM55:
	call	cprintf@PLT
	.stabn	68,0,81,.LM56-.LFBB4
.LM56:
	movq	%rbp, %rdi
	movq	%rsp, %rsi
	call	vcprintf@PLT
	.stabn	68,0,82,.LM57-.LFBB4
.LM57:
	leaq	.LC4(%rip), %rdi
	xorl	%eax, %eax
	call	cprintf@PLT
	jmp	.L21
	.cfi_endproc
.LFE36:
	.size	_panic, .-_panic
	.stabs	"ap:(0,7)=(0,8)=(0,9)=ar(0,10)=r(0,10);0;-1;;0;0;(0,11)=xs__va_list_tag:",128,0,0,0
	.stabs	"va_list:t(0,7)",128,0,0,0
	.stabs	"__builtin_va_list:t(0,8)",128,0,0,0
	.stabs	"__va_list_tag:t(0,11)=s24gp_offset:(0,4),0,32;fp_offset:(0,4),32,32;overflow_arg_area:(0,12)=*(0,1),64,64;reg_save_area:(0,12),128,64;;",128,0,0,0
	.stabn	192,0,0,.LFBB4-.LFBB4
	.stabn	224,0,0,.Lscope4-.LFBB4
.Lscope4:
	.section	.rodata.str1.1
.LC5:
	.string	"kernel warning at %s:%d: "
	.text
	.p2align 4
	.stabs	"_warn:F(0,1)",36,0,0,_warn
	.stabs	"file:P(0,5)",64,0,0,10
	.stabs	"line:P(0,2)",64,0,0,11
	.stabs	"fmt:P(0,5)",64,0,0,6
	.globl	_warn
	.type	_warn, @function
_warn:
	.stabn	68,0,94,.LM58-.LFBB5
.LM58:
.LFBB5:
.LFB37:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %r10
	movl	%esi, %r11d
	movq	%rdx, %rbp
	subq	$208, %rsp
	.cfi_def_cfa_offset 224
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	testb	%al, %al
	je	.L26
	movaps	%xmm0, 80(%rsp)
	movaps	%xmm1, 96(%rsp)
	movaps	%xmm2, 112(%rsp)
	movaps	%xmm3, 128(%rsp)
	movaps	%xmm4, 144(%rsp)
	movaps	%xmm5, 160(%rsp)
	movaps	%xmm6, 176(%rsp)
	movaps	%xmm7, 192(%rsp)
.L26:
	.stabn	68,0,94,.LM59-.LFBB5
.LM59:
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	.stabn	68,0,97,.LM60-.LFBB5
.LM60:
	leaq	224(%rsp), %rax
	.stabn	68,0,98,.LM61-.LFBB5
.LM61:
	movq	%r10, %rsi
	movl	%r11d, %edx
	.stabn	68,0,97,.LM62-.LFBB5
.LM62:
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	.stabn	68,0,98,.LM63-.LFBB5
.LM63:
	leaq	.LC5(%rip), %rdi
	.stabn	68,0,97,.LM64-.LFBB5
.LM64:
	movq	%rax, 16(%rsp)
	.stabn	68,0,98,.LM65-.LFBB5
.LM65:
	xorl	%eax, %eax
	.stabn	68,0,97,.LM66-.LFBB5
.LM66:
	movl	$24, (%rsp)
	movl	$48, 4(%rsp)
	.stabn	68,0,98,.LM67-.LFBB5
.LM67:
	call	cprintf@PLT
	.stabn	68,0,99,.LM68-.LFBB5
.LM68:
	movq	%rbp, %rdi
	movq	%rsp, %rsi
	call	vcprintf@PLT
	.stabn	68,0,100,.LM69-.LFBB5
.LM69:
	xorl	%eax, %eax
	leaq	.LC4(%rip), %rdi
	call	cprintf@PLT
	.stabn	68,0,102,.LM70-.LFBB5
.LM70:
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L29
	addq	$208, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L29:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE37:
	.size	_warn, .-_warn
	.stabs	"ap:(0,7)",128,0,0,0
	.stabn	192,0,0,.LFBB5-.LFBB5
	.stabn	224,0,0,.Lscope5-.LFBB5
.Lscope5:
	.globl	panicstr
	.bss
	.align 8
	.type	panicstr, @object
	.size	panicstr, 8
panicstr:
	.zero	8
	.stabs	"panicstr:G(0,5)",32,0,0,0
	.text
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
