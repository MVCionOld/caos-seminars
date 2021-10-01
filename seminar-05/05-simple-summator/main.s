    .text
    .global	main
main:
    sub sp, sp, #16
	str	lr, [sp, #8]

	ldr	x0, =FORMAT_STRING_SCANF
	mov x1, sp
	add x2, sp, #4
	bl	scanf
	ldr x1, [sp]
	ldr x2, [sp, 4]

    add x3, x1, x2
    ldr	x0, =FORMAT_STRING_PRINTF
    bl	printf

    ldr	lr, [sp, #8]
    add sp, sp, #16

	mov	x0, 0
	ret

    .section    .rodata
FORMAT_STRING_SCANF:
    .string "%d %d"
FORMAT_STRING_PRINTF:
    .string	"%d + %d = %d\n"
