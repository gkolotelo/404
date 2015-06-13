@ +----------------------------------------+
@ | MC404 Turma E - 2015 semestre 1        |
@ | Trabalho 2 - Simulador de IAS para ARM |
@ +----------------------------------------+
@ | Guilherme Kairalla Kolotelo            |
@ | 137943 Alexandre Seidy Ioshisaqui      |
@ +----------------------------------------+

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
    text_scanf_mask:            .asciz "%[^\n]s"
    @ args: addr, op1, op1_addr, op2, op2_addr -> AAA DD DDD DD DDD
    @ input cal also be data, however it'll have the same format above
    deprecated_text_scanf_mask:            .asciz "%X %X %X %X %X" 
    temp_sf_mask: .asciz "%s"
    temp_pf_mask:.asciz "read: %s\n"
    temphex_pf_mask:.asciz "read: %X\n"

strtol_end_addr: .word 0x0

.text

main:
    @ preallocate 20 bytes for 20 characters + 2*4*1024 bytes for IAS memory (1024 bytes @ 64bit each)
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
    sub sp, sp, #20
    sub sp, sp, #8192

    @ Bloco de leitura
    bl read_line
    bl read_hex_input

    @ Bloco de execucao
    @ printf("A simulacao ta comecando.")
    bl exec_mem_map_begin
    @ printf("A simulacao terminou.")


    pop {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ If SP needs to be back to original location, add:
    add sp, sp, #20 @ fp-19 points to last position (where 1st char will end up)
    add sp, sp, #8192

    mov r0, #0
    mov r7, #1
    svc 0x00
    




read_line:
    push {lr}

    ldr r0, =text_scanf_mask
    @ set starting addr on lower stack addresses, since scanf goes up the stack
    mov r1, fp
    sub r1, r1, #19

    bl scanf

    ldr r0, =temp_pf_mask
    mov r1, fp
    sub r1, r1, #19

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
    sub r0, r0, #19             @ (strtol arg) str: fp-19, points to 1st char read by scanf
    ldr r1, =strtol_end_addr    @ (strtol arg) endPptr: will be stored in 'strtol_end_addr'
    mov r2, #16                 @ (strtol arg) base: base 16 (hex)
    bl strtol                   @ Call strtol
    mov _addr, r0               @ Move result to corresponding register (addr, op1, op1_addr, op2, op2_addr)
    @ _op1
    ldr r0, =strtol_end_addr    @ (strtol arg) str: Now strtol gets addr from 'strtol_end_addr' returned last iteration
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov _op1, r0
    @ _op1_addr
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov _op1_addr, r0
    @ _op2
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov _op2, r0
    @ _op2_addr
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov _op2_addr, r0
    @ Finished

    pop {lr}
    bx lr


build_memory_map:
    @ MemoryMap definition:
    @ addr ponts to pair of consecutive stack addresses:
    @   addr 0 -> [32][32]
    @   addr 1 -> [32][32]
    @
    @ View on stack:
    @   addr 0 -> [32] -> stack addr 0x105 (fp + 20)
    @   addr 0 -> [32] -> stack addr 0x104
    @   addr 1 -> [32] -> stack addr 0x103
    @   addr 1 -> [32] -> stack addr 0x102 ... (until sp)
    @
    @   [32][32] -> [000 DD DDD][DD DDD 000]
    @   ...
    for:
    bl read_line
    bl read_hex_input
    bl add_to_memory @ uses _addr, _op1, _op1_add, _op2, _op2_addr 

add_to_memory:
    @ uses _addr, _op1, _op1_add, _op2, _op2_addr from 'read_hex_input'
    @ uses strd 
    @ lsl _addr, _addr, #20 addr unused
    @
    @ Converts 40 bits read from input into 2 registers, as defined above, then
    @ inserts into stack at fp+20+2*_addr
    lsl _op1, _op1, #12
    lsl _op2, _op2, #24
    lsl _op2_addr, _op2_addr, #12
    orr r0, _op1, _op1_addr
    orr r1, _op2, _op2_addr
    strd r1, r0, 
    

test_addr:
    push {lr}

    ldr r0, =1023
    cmp
    blt test_addr_exit
    mov r1, r0
    ldr r0, =text_invalid_addr
    bl printf


test_addr_exit:
    pop {lr}
    bx lr


@ Bloco de execucao do mapa de memoria
exec_mem_map_begin:
    push{lr}
    @ Inicia variaveis
    @ ac, mq, pc, jump, side, error, inst

    @   printf("Estado inicial")
    @   printf("AC:, MQ:, PC:")
    @   printf("-----------")

    @   while(true):
    @       for(j=left, (conditions), j++):
    @       (conditions) := side:(left,right) && jump:false && error:false
    @           switch(OP_CODE):
exec_loop:

    @ switch(OP_CODE)
    cmp _addr, 0x01  @ LOAD
    beq op_load

    cmp _addr, 0x09  @ LOADMQM
    beq op_loadmqm

    cmp _addr, 0x0A  @ LOADMQ
    beq op_loadmq

    cmp _addr, 0x03  @ LOADABS
    beq op_loadabs

    cmp _addr, 0x02  @ LOADN
    beq op_loadn

    cmp _addr, 0x21  @ STOR
    beq op_stor

    cmp _addr, 0x12  @ STORL
    beq op_storl

    cmp _addr, 0x13  @ STORR
    beq op_storr

    cmp _addr, 0x05  @ ADD
    beq op_add

    cmp _addr, 0x07  @ ADDABS
    beq op_addabs

    cmp _addr, 0x06  @ SUB
    beq op_sub

    cmp _addr, 0x08  @ SUBABS
    beq op_subabs

    cmp _addr, 0x0B  @ MUL
    beq op_mul

    cmp _addr, 0x0C  @ DIV
    beq op_div

    cmp _addr, 0x15  @ RSH
    beq op_rsh

    cmp _addr, 0x14  @ LSH
    beq op_lsh

    cmp _addr, 0x0D  @ JUMPL
    beq op_jumpl

    cmp _addr, 0x0E  @ JUMPR
    beq op_jumpr

    cmp _addr, 0x0F  @ JUMPPL
    beq op_jumppl

    cmp _addr, 0x10  @ JUMPPR
    beq op_jumppr

    b exec_mem_map_end
    @ default:error

op_case_end:



exec_mem_map_end:
    b op_case_end

op_load:
    b op_case_end

op_loadmqm:
    b op_case_end

op_loadmq:
    b op_case_end

op_loadabs:
    b op_case_end

op_loadn:
    b op_case_end

op_stor:
    b op_case_end

op_storl:
    b op_case_end

op_storr:
    b op_case_end

op_add:
    b op_case_end

op_addabs:
    b op_case_end

op_sub:
    b op_case_end

op_subabs:
    b op_case_end

op_mul:
    b op_case_end

op_div:
    b op_case_end

op_rsh:
    b op_case_end

op_lsh:
    b op_case_end

op_jumpl:
    b op_case_end

op_jumpr:
    b op_case_end

op_jumppl:
    b op_case_end

op_jumppr:
    b op_case_end












