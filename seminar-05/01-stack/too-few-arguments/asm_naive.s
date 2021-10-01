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
	ldr	x1, [sp, 56]
	ldr	x0, [sp, 48]
	add	x1, x1, x0
	ldr	x0, [sp, 40]
	add	x1, x1, x0
	ldr	x0, [sp, 32]
	add	x1, x1, x0
	ldr	x0, [sp, 24]
	add	x1, x1, x0
	ldr	x0, [sp, 16]
	add	x1, x1, x0
	ldr	x0, [sp, 8]
	add	x1, x1, x0
	ldr	x0, [sp]
	add	x1, x1, x0
	ldr	x0, [sp, 64]
	add	x1, x1, x0
	ldr	x0, [sp, 72]
	add	x1, x1, x0
	ldr	x0, [sp, 80]
	add	x1, x1, x0
	ldr	x0, [sp, 88]
	add	x0, x1, x0
	add	sp, sp, 64
	ret
