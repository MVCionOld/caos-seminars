    .text
    .global	main
main:
    sub sp, sp, #16
	str	lr, [sp]

	mov w1, #4

	ldr	x0, =FORMAT_STRING_LABEL

	bl	printf

    ldr	lr, [sp]
    add sp, sp, #16

	mov	x0, 0
	ret

    .section    .rodata
FORMAT_STRING_LABEL:
    .string	"2 + 2 = %d\n"