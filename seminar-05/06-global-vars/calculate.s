    .data
    .global R
R:  .space 8

    .text
    .global calculate

    // calculate R = (A * B) + (C * D)
calculate:
    adr x0, .Lexterns   // x0 <-- address of .Lexterns, where addressed of vars stored
    ldr x1, [x0]        // x1 <-- &A
    ldr x2, [x0, 8]     // x2 <-- &B
    ldr x3, [x0, 16]    // x3 <-- &C
    ldr x4, [x0, 24]    // x4 <-- &D
    ldr x5, [x0, 32]    // x5 <-- &R
    ldr x1, [x1]        // x1 <-- *x1 = A
    ldr x2, [x2]        // x2 <-- *x2 = B
    ldr x3, [x3]        // x3 <-- *x3 = C
    ldr x4, [x4]        // x4 <-- *x4 = D
    mul x6, x1, x2
    mul x7, x3, x4
    add x8, x6, x7
    str x8, [x5]
    ret

.Lexterns:
    .quad   A
    .quad   B
    .quad   C
    .quad   D
    .quad   R