@ +----------------------------------------+
@ | MC404 Turma E - 2015 semestre 1        |
@ | Trabalho 2 - Simulador de IAS para ARM |
@ +----------------------------------------+
@ | 135964 Guilherme Kairalla Kolotelo            |
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
    @ args: addr, op1, op1addr, op2, op2addr -> AAA DD DDD DD DDD
    @ input cal also be data, however it'll have the same format above
    deprecated_text_scanf_mask:            .asciz "%X %X %X %X %X" 
    temp_sf_mask: .asciz "%s"
    temp_pf_mask:.asciz "read: %s\n"
    temphex_pf_mask:.asciz "read: %X\n"

strtol_end_addr: .word 0x0

.text

main:
    @ preallocate 20 bytes for 20 characters + 2*4*1024 bytes for IAS memory (1024 bytes @ 64bits each)
    @ fp will point to start o MemoryMap (IAS memory entry every 8 bytes)
    @ fp - 8191 will point to 1st char of buffer
    push {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ Cannot writeback to SP! Do we even need to care for SP?

    @ Define register names:
    ac         .req r8
    mq         .req r9
    pc_ias         .req r10
    addr       .req r3
    op1         .req r4
    op1_addr   .req r5
    op2         .req r6
    op2_addr   .req r7

    @ Set FP
    mov fp, sp
    sub fp, fp, #4

    @ Allocate memory on stack
    @ Stack: 1st 8192 bytes: MemoryMap, remaining 20 bytes: string buffer
    sub sp, sp, #8192
    sub sp, sp, #20

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
    @ addr, op1, op1addr, op2, op2addr will be set respectively 

    push {lr}
    @ _addr
    mov r0, fp      
    sub r0, r0, #8192           @ (strtol arg) str: fp-8191, points to 1st char read by scanf
    add r0, r0, #1              @ must add 1, since immediate is higher than 12bits (-8192+1)
    ldr r1, =strtol_end_addr    @ (strtol arg) endPptr: will be stored in 'strtol_end_addr'
    mov r2, #16                 @ (strtol arg) base: base 16 (hex)
    bl strtol                   @ Call strtol
    mov addr, r0                @ Move result to corresponding register (addr, op1, op1addr, op2, op2_addr)
    @ op1 
    ldr r0, =strtol_end_addr    @ (strtol arg) str: Now strtol gets addr from 'strtol_end_addr' returned last iteration
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov op1 , r0
    @ op1 _addr
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov op1_addr, r0
    @ op2 
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov op2 , r0
    @ op2 _addr
    ldr r0, =strtol_end_addr
    ldr r0, [r0]
    ldr r1, =strtol_end_addr
    mov r2, #16
    bl strtol
    mov op2_addr, r0
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
    bl add_to_memory @ uses addr, op1 , op1 _add, op2 , op2 addr 

add_to_memory:
    @ uses addr, op1 , op1 _add, op2 , op2 addr from 'read_hex_input'
    @ uses strd 
    @ lsl addr, addr, #20 addr unused
    @
    @ Converts 40 bits read from input into 2 registers, as defined above, then
    @ inserts into stack at fp+20+2*_addr

    b test_addr

    lsl op1, op1, #12
    lsl op2, op2, #24
    lsl op2_addr, op2_addr, #12
    orr r1, op1, op1_addr
    orr r0, op2, op2_addr
    add addr, addr, #20        @ addr = addr + 20, will be used below to compensate
    strd r0, r1, [fp, -addr]   @ Store [r0][r1] to [fp-20-_addr][fp-20-addr - 4]
    

test_addr:
    @ Tests if address at r0 is less then 1023 (0x3FF)
    push {lr}
    @ r0 contains address to be tested
    ldr r1, =1023
    cmp r0, r1  @ if r0-1023 < 0 the address is valid
    blt test_addr_exit
    mov r1, r0
    ldr r0, =text_invalid_addr
    bl printf


test_addr_exit:
    pop {lr}
    bx lr


@ Bloco de execucao do mapa de memoria
exec_mem_map_begin:
    push {lr}
    @ Inicia variaveis
    @ ac, mq, pc, jump, side, error, inst

    @   printf("Estado inicial")
    @   printf("AC:, MQ:, PC:")
    @   printf("-----------")

    @ Overview do loop:
    @ | while(true):
    @ |  |-> for(j=left, (conditions), j++):
    @ |  |   (conditions) := side:(left,right) && jump:false && error:false
    @ |      |-> switch(OP_CODE): seleciona operacao
    @ |      |-> executa ate encontrar operacao invalida

exec_loop_begin:
    @ printf("Executando instrucao")
    @ if (side == left): left-shift 20bits + mascara 0xFFFFF
    @ else: mascara 0xFFFFF

    @ switch(OP_CODE)
    cmp addr, #0x01  @ case LOAD:
    beq op_load

    cmp addr, #0x09  @ case LOADMQM:
    beq op_loadmqm

    cmp addr, #0x0A  @ case LOADMQ:
    beq op_loadmq

    cmp addr, #0x03  @ case LOADABS:
    beq op_loadabs

    cmp addr, #0x02  @ case LOADN:
    beq op_loadn

    cmp addr, #0x21  @ case STOR:
    beq op_stor

    cmp addr, #0x12  @ case STORL:
    beq op_storl

    cmp addr, #0x13  @ case STORR:
    beq op_storr

    cmp addr, #0x05  @ case ADD:
    beq op_add

    cmp addr, #0x07  @ case ADDABS:
    beq op_addabs

    cmp addr, #0x06  @ case SUB:
    beq op_sub

    cmp addr, #0x08  @ case SUBABS:
    beq op_subabs

    cmp addr, #0x0B  @ case MUL:
    beq op_mul

    cmp addr, #0x0C  @ case DIV:
    beq op_div

    cmp addr, #0x15  @ case RSH:
    beq op_rsh

    cmp addr, #0x14  @ case LSH:
    beq op_lsh

    cmp addr, #0x0D  @ case JUMPL:
    beq op_jumpl

    cmp addr, #0x0E  @ case JUMPR:
    beq op_jumpr

    cmp addr, #0x0F  @ case JUMPPL:
    beq op_jumppl

    cmp addr, #0x10  @ case JUMPPR:
    beq op_jumppr

    @ default:error
    @ fprintf(stderr, "Erro! Instrucao invalida.")
    @ error := true

    b exec_mem_map_end

op_case_end:
    @   if(error == true) exec_mem_map_end
    @   else:
    @      ac &= 0xFFFFFFFFFF;
    @      mq &= 0xFFFFFFFFFF;
    @      pc &= 0xFFFFFFFFFF;
    @   printf("+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n", ac, mq, pc);
    @   printf("--------------------------------------------------------------\n");

    @ j < 2 && jump == 0 && error == 0
    @ := side:(left,right) && jump:false && error:false
    @ Loop conditions
@    cmp rSide, #1          @ side is left or right (j < 2)
@    bgt exec_loop_end
@    cmp rJump, #1          @ jump == false
@    beq exec_loop_end
@    cmp rError, #1         @ error == false
@    beq exec_loop_end

@    b exec_loop_begin

exec_loop_end:
    @   if (jump == false):
    @       pc := pc + 1
    @       first := 0
    @   else: jump := 0

exec_mem_map_end:
    pop {lr}
    bx lr

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












