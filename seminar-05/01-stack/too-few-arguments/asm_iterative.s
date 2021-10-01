	.text
	.global	few_arguments_function_sum

few_arguments_function_sum:
	sub	sp, sp, #64

	str	x0, [sp, 56]
	str	x1, [sp, 48]
	str	x2, [sp, 40]
	str	x3, [sp, 32]
	str	x4, [sp, 24]
	str	x5, [sp, 16]
	str	x6, [sp, 8]
	str	x7, [sp]

    mov x0, #0
    mov x1, #6

    .SUM_LOOP:
        ldr x2, [sp]
        ldr x3, [sp, 8]
        add sp, sp, #16
        add x0, x0, x2
        add x0, x0, x3
        sub x1, x1, #1
        cmp x1, #0
        bgt .SUM_LOOP

	sub	sp, sp, #32
	ret
