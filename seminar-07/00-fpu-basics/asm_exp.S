    .intel_syntax noprefix
    .text
    .globl  my_exp
my_exp:
    sub     esp, 12
    mov     eax, 2
    fld     QWORD PTR [esp+16]
    fld1
    fld1
    fld1
    fld1
    // On stack (0-4): fac=1, part=1, old_result=1, xn=1, x
.loop:
    mov     DWORD PTR [esp+4], eax
    add     eax, 1
    fxch    st(1)             // On stack (0-4): part, fac, old_result, xn, x
    fadd    st(0), st(2)      // On stack (0-4): result=part + old_result, fac, old_result, xn, x
    fild    DWORD PTR [esp+4] // On stack (0-5): i, result, fac, old_result, xn, x
    fmulp   st(2), st(0)      // On stack (0-4): result, fac=fac*i, old_result, xn, x
    fxch    st(3)             // On stack (0-4): xn, fac, old_result, result, x
    fmul    st(0), st(4)      // On stack (0-4): xn=xn*x, fac, old_result, result, x
    fld     st(0)             // On stack (0-5): xn, xn, fac, old_result, result, x
    fdiv    st(0), st(2)      // On stack (0-5): npart=xn/fac, xn, fac, old_result, result, x
    fxch    st(4)             // On stack (0-5): result, xn, fac, old_result, npart, x
    fcomi   st(0), st(3)      // On stack (0-5): result, xn, fac, old_result, npart, x (compare result and old_result)
    fstp    st(3)             // On stack (0-4): xn, fac, result, npart, x
    je      .finish           // if result == old_result then go to .finish
    fxch    st(3)             // On stack (0-4): npart, fac, result, xn, x
    fxch    st(1)             // On stack (0-4): fac, npart, result, xn, x
    jmp     .loop
.finish:
    fxch    st(2)             // On stack (0-4): result, npart, fac, xn, x
    add     esp, 12
    ret