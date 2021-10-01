// Этот код не работает
// Прочитать почему можно здесь:
// https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/using-the-stack-in-aarch64
-implementing-push-and-pop

	.text
	.global	asm_fibonacci
asm_fibonacci:

    mov x1, #1
    push {xzr}
    push {x1}

    mov x1, #1 // int i = 0

    .FIB_LOOP_BEGIN:
        cmp x1, x0
        beq .FIB_LOOP_END

        pop {x3}          // x3 <- curr
        pop {x2}          // x2 <- prev
        add x4, x2, x3  // x4 <- prev + curr

        push {x3}
        push {x4}

        add x1, x1, #1

        b .FIB_LOOP_BEGIN

    .FIB_LOOP_END:
        pop {x0}
        pop {x1} // to recover stack pointer to initial state
        ret
