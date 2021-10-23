	.file	"main.c"
	.intel_syntax noprefix
#APP
	
syscall:
    /* Function arguments: rdi, rsi, rdx, rcx, r8, r9 */
    /* Syscall arguments: rax (syscall num), rdi, rsi, rdx, r10, r8, r9.*/
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov r10, r8
    mov r8, r9
    syscall
    ret

	
.globl _start
_start:
    sub rsp, 8
    jmp main

#NO_APP
	.text
	.p2align 4,,15
	.globl	my_exit
	.type	my_exit, @function
my_exit:
	mov	esi, edi
	xor	eax, eax
	mov	edi, 60
	jmp	syscall
	.size	my_exit, .-my_exit
	.p2align 4,,15
	.globl	write
	.type	write, @function
write:
	mov	ecx, edx
	xor	eax, eax
	mov	rdx, rsi
	mov	esi, edi
	mov	edi, 1
	jmp	syscall
	.size	write, .-write
	.p2align 4,,15
	.globl	int_to_s
	.type	int_to_s, @function
int_to_s:
	test	edi, edi
	mov	r9, rdx
	mov	ecx, edi
	je	.L9
	xor	r8d, r8d
	mov	r10d, 1717986919
	.p2align 4,,10
	.p2align 3
.L6:
	mov	eax, ecx
	sar	ecx, 31
	add	r8d, 1
	imul	r10d
	sar	edx, 2
	sub	edx, ecx
	mov	ecx, edx
	jne	.L6
	movsx	rax, r8d
	mov	rdx, rax
.L4:
	mov	BYTE PTR [rsi+rdx], 0
	xor	ecx, ecx
	add	rsi, rax
	mov	r10d, -858993459
	.p2align 4,,10
	.p2align 3
.L8:
	mov	eax, edi
	mul	r10d
	shr	edx, 3
	lea	eax, [rdx+rdx*4]
	add	eax, eax
	sub	edi, eax
	add	edi, 48
	mov	BYTE PTR [rsi-1+rcx], dil
	sub	rcx, 1
	mov	edi, edx
	mov	eax, ecx
	neg	eax
	cmp	r8d, eax
	jg	.L8
	mov	DWORD PTR [r9], r8d
	ret
.L9:
	mov	edx, 1
	mov	r8d, 1
	mov	eax, 1
	jmp	.L4
	.size	int_to_s, .-int_to_s
	.p2align 4,,15
	.globl	s_to_int
	.type	s_to_int, @function
s_to_int:
	movsx	edx, BYTE PTR [rdi]
	lea	eax, [rdx-48]
	cmp	al, 9
	mov	eax, 0
	ja	.L14
	.p2align 4,,10
	.p2align 3
.L13:
	lea	eax, [rax+rax*4]
	add	rdi, 1
	lea	eax, [rdx-48+rax*2]
	movsx	edx, BYTE PTR [rdi]
	lea	ecx, [rdx-48]
	cmp	cl, 9
	jbe	.L13
	rep ret
.L14:
	rep ret
	.size	s_to_int, .-s_to_int
	.p2align 4,,15
	.globl	print_int
	.type	print_int, @function
print_int:
	sub	rsp, 40
	test	esi, esi
	mov	r9d, esi
	je	.L21
	mov	r8d, esi
	xor	ecx, ecx
	mov	esi, 1717986919
	.p2align 4,,10
	.p2align 3
.L18:
	mov	eax, r8d
	sar	r8d, 31
	add	ecx, 1
	imul	esi
	sar	edx, 2
	sub	edx, r8d
	mov	r8d, edx
	jne	.L18
	movsx	r10, ecx
	mov	rax, r10
.L16:
	mov	BYTE PTR [rsp+rax], 0
	xor	r8d, r8d
	add	r10, rsp
	mov	esi, -858993459
	.p2align 4,,10
	.p2align 3
.L20:
	mov	eax, r9d
	mul	esi
	shr	edx, 3
	lea	eax, [rdx+rdx*4]
	add	eax, eax
	sub	r9d, eax
	add	r9d, 48
	mov	BYTE PTR [r10-1+r8], r9b
	sub	r8, 1
	mov	r9d, edx
	mov	eax, r8d
	neg	eax
	cmp	ecx, eax
	jg	.L20
	mov	rdx, rsp
	mov	esi, edi
	xor	eax, eax
	mov	edi, 1
	call	syscall
	add	rsp, 40
	ret
.L21:
	mov	eax, 1
	mov	ecx, 1
	mov	r10d, 1
	jmp	.L16
	.size	print_int, .-print_int
	.p2align 4,,15
	.globl	print_s
	.type	print_s, @function
print_s:
	cmp	BYTE PTR [rsi], 0
	mov	rdx, rsi
	je	.L26
	lea	rax, [rsi+1]
	xor	ecx, ecx
	.p2align 4,,10
	.p2align 3
.L25:
	add	rax, 1
	add	ecx, 1
	cmp	BYTE PTR [rax-1], 0
	jne	.L25
.L24:
	mov	esi, edi
	xor	eax, eax
	mov	edi, 1
	jmp	syscall
.L26:
	xor	ecx, ecx
	jmp	.L24
	.size	print_s, .-print_s
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"123456"
.LC1:
	.string	"Look at this value: "
.LC2:
	.string	"\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
	sub	rsp, 40
	movabs	rax, 8031924123371070792
	mov	ecx, 35
	mov	QWORD PTR [rsp], rax
	movabs	rax, 7885647255504710770
	mov	edx, OFFSET FLAT:hello_s
	mov	QWORD PTR [rsp+8], rax
	movabs	rax, 7809632576447129376
	mov	esi, 1
	mov	QWORD PTR [rsp+16], rax
	mov	edi, 1
	xor	eax, eax
	mov	DWORD PTR [rsp+24], 169944940
	mov	BYTE PTR [rsp+28], 0
	call	syscall
	mov	rdx, rsp
	mov	ecx, 28
	mov	esi, 1
	mov	edi, 1
	xor	eax, eax
	call	syscall
	mov	esi, OFFSET FLAT:.LC1
	mov	edi, 1
	call	print_s
	mov	esi, 10050042
	mov	edi, 1
	call	print_int
	mov	esi, OFFSET FLAT:.LC2
	mov	edi, 1
	call	print_s
	mov	esi, OFFSET FLAT:.LC1
	mov	edi, 1
	call	print_s
	mov	edx, OFFSET FLAT:.LC0
	mov	eax, 49
	xor	esi, esi
	.p2align 4,,10
	.p2align 3
.L29:
	lea	ecx, [rsi+rsi*4]
	add	rdx, 1
	lea	esi, [rax-48+rcx*2]
	movsx	eax, BYTE PTR [rdx]
	lea	ecx, [rax-48]
	cmp	cl, 9
	jbe	.L29
	mov	edi, 1
	call	print_int
	mov	esi, OFFSET FLAT:.LC2
	mov	edi, 1
	call	print_s
	xor	esi, esi
	mov	edi, 60
	xor	eax, eax
	call	syscall
	add	rsp, 40
	ret
	.size	main, .-main
	.globl	hello_s_size
	.section	.rodata
	.align 4
	.type	hello_s_size, @object
	.size	hello_s_size, 4
hello_s_size:
	.long	36
	.globl	hello_s
	.align 32
	.type	hello_s, @object
	.size	hello_s, 36
hello_s:
	.string	"Hello world from function 'write'!\n"
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
