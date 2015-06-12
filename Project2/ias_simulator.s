@ Trabalho 2
@
@ 2015
@ Guilherme Kairalla Kolotelo

.globl main

.extern printf
.extern scanf
.extern strtol

@ Reference:
@ OP_LOAD #0x01
@ OP_LOADMQM #0x09
@ OP_LOADMQ #0x0A
@ OP_LOADABS #0x03
@ OP_LOADN #0x02
@ OP_STOR #0x21
@ OP_STORL #0x12
@ OP_STORR #0x13
@ OP_ADD #0x05
@ OP_ADDABS #0x07
@ OP_SUB #0x06
@ OP_SUBABS #0x08
@ OP_MUL #0x0B
@ OP_DIV #0x0C
@ OP_RSH #0x15
@ OP_LSH #0x14
@ OP_JUMPL #0x0D
@ OP_JUMPR #0x0E
@ OP_JUMPPL #0x0F
@ OP_JUMPPR #0x10

.data
    @ Errors and messages
    text_sim_started:           .asciz "\nIASIM: A simulacao esta comecando.\n\n"
    text_sim_finished:          .asciz "\nIASIM: A simulacao terminou.\n\n"
    text_invalid_addr:          .asciz "IASIM: Erro! Endereco invalido de numero %04X.\n"           @ args: addr
    @ Simulation messages
    text_init_state:            .asciz "@ Estado inicial:\n"
    text_reg_info:              .asciz "+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n"  @ args: ac & 0xFFFFFFFFFF, mq & 0xFFFFFFFFFF, pc & 0xFFFFFFFFFF
    text_executing_at_addr:     .asciz "@ Executando instrucao no endereco %010X "                  @ args: pc
    text_intruction_at_left:    .asciz "(instrucao a esquerda)\n"
    text_intruction_at_right:   .asciz "(instrucao a direita)\n"
    text_curr_location:         .asciz "+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n"  @ args: ac, mq, pc
    text_separator:             .asciz "--------------------------------------------------------------\n"
    @ opcodes   
    text_OP_LOAD:               .asciz "@ LOAD M(X), X = 0x%04X\n"          @ args: addr
    text_OP_LOADMQM:            .asciz "@ LOAD MQ,M(X), X = 0x%04X\n"       @ args: addr
    text_OP_LOADMQ:             .asciz "@ LOAD MQ, X = 0x%04X\n"            @ args: addr
    text_OP_LOADABS:            .asciz "@ LOAD |(M(X)|, X = 0x%04X\n"       @ args: addr
    text_OP_LOADN:              .asciz "@ LOAD -(M(X)), X = 0x%04X\n"       @ args: addr
    text_OP_STOR:               .asciz "@ STOR M(X), X = 0x%04X\n"          @ args: addr
    text_OP_STORL:              .asciz "@ STOR M(X,8:19), X =0x%04X\n"      @ args: addr
    text_OP_STORR:              .asciz "@ STOR M(X,28:39), X =0x%04X\n"     @ args: addr
    text_OP_ADD:                .asciz "@ ADD M(X), X = 0x%04X\n"           @ args: addr
    text_OP_ADDABS:             .asciz "@ ADD |M(X)|, X = 0x%04X\n"         @ args: addr
    text_OP_SUB:                .asciz "@ SUB M(X), X = 0x%04X\n"           @ args: addr
    text_OP_SUBABS:             .asciz "@ SUB |M(X)|, X = 0x%04X\n"         @ args: addr
    text_OP_MUL:                .asciz "@ MUL M(X), X = 0x%04X\n"           @ args: addr
    text_OP_DIV:                .asciz "@ DIV M(X), X = 0x%04X\n"           @ args: addr
    text_OP_RSH:                .asciz "@ RSH, X = 0x%04X\n"                @ args: addr
    text_OP_LSH:                .asciz "@ LSH, X = 0x%04X\n"                @ args: addr
    text_OP_JUMPR:              .asciz "@ JUMP M(X,0:19), X = 0x%04X\n"     @ args: addr
    text_OP_JUMPL:              .asciz "@ JUMP M(X,20:39), X = 0x%04X\n"    @ args: addr
    text_OP_JUMPPL:             .asciz "@ JUMP+ M(X,0:19), X = 0x%04X\n"    @ args: addr
    text_OP_JUMPPr:             .asciz "@ JUMP+ M(X,20:39), X = 0x%04X\n"   @ args: addr
    @ opcode-specific errors and messages
    text_OP_Err_div_zero:       .asciz "IASIM: Erro! Divisao por zero.\n"
    text_OP_jump:               .asciz "@ Salto realizado\n"
    text_OP_invalid:            .asciz "IASIM: Erro! Instrucao invalida com opcode %02X.\n"         @ args: addr
    @ scanf mask:
    text_scanf_mask:            .asciz "%s"
    deprecated_text_scanf_mask:            .asciz "%X %X %X %X %X" @ args: addr, op1, op1_addr, op2, op2_addr -> AAA DD DDD DD DDD
    temp_sf_mask: .asciz "%s"
    temp_pf_mask:.asciz "read: %s\n"

.text

main:
    @ preallocate 80 bytes for 20 characters + 2*4*1024 bytes for IAS memory (1024 bytes @ 64bit each)
    @ fp will point to char array
    @ fp + 80 to fp + 80 + 8192 will point to IAS memory
    push {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ Cannot writeback to SP! Do we even need to care for SP?
    @ Define register names:
    _ac .req r8
    _mq .req r9
    _pc .req r10
    _addr       .req r3
    _op1        .req r4
    _op1_addr   .req r5
    _op2        .req r6
    _op2_addr   .req r7
    @ Set FP
    mov fp, sp
    sub fp, fp, #4
    @ Allocate memory on stack
    sub sp, sp, #80
    sub sp, sp, #8192




    bl build_memory_map




    pop {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ If SP needs to be back to original location, add:
    add sp, sp, #80
    add sp, sp, #8192

    mov r0, #0
    mov r7, #1
    svc 0x00
    




read_line:
    push {lr}

    ldr r0, =text_scanf_mask
    mov r1, fp

    bl scanf

    ldr r0, =temp_pf_mask
    mov r1, fp

    bl printf

    pop {lr}
    bx lr

read_hex_input:
    @ r0 has address on stack where input string is located
    @ strtol will be run 5 times 
    @ addr, op1, op1_addr, op2, op2_addr will be set respectively 

    push {lr}
    @ _addr
    mov r0, fp
    mov r1, fp
    mov r2, #16
    bl strtol
    mov _addr, r0
    @ _op1
    mov r0, fp
    mov r1, fp
    mov r2, #16
    bl strtol
    mov _op1 r0
    @ _op1_addr
    mov r0, fp
    mov r1, fp
    mov r2, #16
    bl strtol
    mov _op1_addr, r0
    @ _op2
    mov r0, fp
    mov r1, fp
    mov r2, #16
    bl strtol
    mov _op2, r0
    @ _op2_addr
    mov r0, fp
    mov r1, fp
    mov r2, #16
    bl strtol
    mov _op2_addr, r0
    @ Finished

build_memory_map:
    @hello from guilherme
    for:
    bl read_line
    bl read_hex_input
    bl add_to_memory @ uses _addr, _op1, _op1_add, _op2, _op2_addr 




test_addr:
    push {lr}

    ldr r0, =1023
    blt test_addr_exit
    mov r1, r0
    ldr r0, =text_invalid_addr
    bl printf
test_addr_exit:
    pop {lr}
    bx lr





















