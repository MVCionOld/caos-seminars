    .text
    .global	main
main:
	ldr	x0, =HELLO_WORLD_LABEL

    sub sp, sp, #16
	str	lr, [sp]
	bl	printf
    ldr	lr, [sp]
    add sp, sp, #16

	mov	x0, 0
	ret

    .section    .rodata
HELLO_WORLD_LABEL:
    .string	"Hello, World!\n"
    // or use .ascii "Hello, World!\n\0"
    // or use .asciz "Hello, World!\n"