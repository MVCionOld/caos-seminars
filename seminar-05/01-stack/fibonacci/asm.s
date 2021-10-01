	.text
	.global	asm_fibonacci
asm_fibonacci:
    sub sp, sp, #16

    mov x1, #1
    str xzr, [sp, 8]
    str x1, [sp]

    mov x1, #1 // int i = 0

    .FIB_LOOP_BEGIN:
        cmp x1, x0
        beq .FIB_LOOP_END

        ldr x2, [sp, 8] // x2 <- prev
        ldr x3, [sp]    // x3 <- curr
        add x4, x2, x3  // x4 <- prev + curr

        str x3, [sp, 8]
        str x4, [sp]

        add x1, x1, #1

        b .FIB_LOOP_BEGIN

    .FIB_LOOP_END:
        ldr x0, [sp]
        add sp, sp, #16
        ret
