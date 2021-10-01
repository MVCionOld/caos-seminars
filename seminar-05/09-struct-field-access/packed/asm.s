   .text
   .global sum_operands
sum_operands:
    ldr x1, [x0, 0]
    ldr x2, [x0, 8]
    add x0, x1, x2
    ret
