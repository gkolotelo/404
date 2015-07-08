.globl main

.text

main:

mov r0, #15
mov r2, #4
@ Division
@ ac(r0) : numerator, mem[x](r2) : denominator
push {r0,r1,r2,r3}
lo .req r0
hi .req r1
den .req r2
sign .req r3
@ Handle input
cmp den, #0
beq div_zero
division:
    @ Make certain that numerator and denominator are positive
    mov sign, #1
    rsblt sign, sign, #0        @ If den < 0 => sign = -sign
    rsblt den, den, #0          @ If den < 0 => den = -den
    cmp lo, #0
    rsblt sign, sign, #0        @ If num < 0 => sign = -sign
    rsblt lo, lo, #0            @ If num < 0 => lo = -lo
    @ If numerator <= denomitator, end.
    cmp lo, den
    movlt hi, lo                @ r1 has remainder
    movlt lo, #0                @ r0 has quotient
    moveq lo, #1
    ble end
    @ Start division
    rsb den, den, #0            @ negate den for the divide loop
    mov hi, #0
    adds lo, lo, lo             @ first loop instruction relocated here
    .rept 32                    @ repeat 32 times
      adcs hi, den, hi, lsl #1
      subcc hi, hi, den
      adcs lo, lo, lo
    .endr
end:
        cmp sign, #0                @ quotient = -quotient if sign < 0
        rsblt lo, lo, #0
        pop {r0,r1,r2,r3}
        @ ac has remainder
        @ mq has quotient
        @b op_case_end
div_zero:
    @push {r0,r1,r2,r3}
    @ldr r0, =text_OP_Err_div_zero @ "Erro! Divisao por zero."
    @bl printf
    @pop {r0,r1,r2,r3}
    @mov r4, #1              @ error = 1
    @b op_case_end
    @pop {r0,r1,r2,r3}
@r0: quotient (lo)
@r1: remainder (hi)

        mov r0,r0
        mov r0,r0



