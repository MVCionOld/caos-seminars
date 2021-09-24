   .text
   .global asm_for_loop_inc
asm_for_loop_inc:
                        // w0 <- N, w1 <- x0
    mov     w2, #0      // int i = 0
.LoopBegin:
    cmp     w0, w2      // if (i == N)
    beq     .LoopEnd    // then break
    add     w1, w1, 1   // else ++x0
    add     w2, w2, 1   // and ++i
    b       .LoopBegin
.LoopEnd:
    mov     w0, w1
    ret
