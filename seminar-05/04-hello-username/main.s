    .text
    .global	main
main:
    sub sp, sp, #16
	str	lr, [sp]

	ldr	x0, =FORMAT_STRING_LABEL
	ldr x1, =USERNAME_LABEL

	bl	printf

    ldr	lr, [sp]
    add sp, sp, #16

	mov	x0, 0
	ret

    .section    .rodata
FORMAT_STRING_LABEL:
    .string	"Hello, %s!\n"
USERNAME_LABEL:
    .string "Donald Trump"