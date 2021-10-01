    .text
    .global	main
main:
    sub sp, sp, #32
	str	lr, [sp, #16]

	ldr	x0, =FORMAT_STRING_SCANF
	mov x1, sp
	add x2, sp, #8
	bl	scanf
	ldr x1, [sp]
	ldr x2, [sp, 8]

    add x3, x1, x2
    ldr	x0, =FORMAT_STRING_PRINTF
    bl	printf

    ldr	lr, [sp, #16]
    add sp, sp, #32

	mov	x0, 0
	ret

    .section    .rodata
FORMAT_STRING_SCANF:
    .string "%d %d"
FORMAT_STRING_PRINTF:
    .string	"%d + %d = %d\n"
