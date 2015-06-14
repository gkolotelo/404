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
    text_scanf_mask:            .asciz "%[^\n]s\n"
    @ args: addr, op1, op1addr, op2, op2addr -> AAA DD DDD DD DDD
    @ input cal also be data, however it'll have the same format above
    deprecated_text_scanf_mask:            .asciz "%X %X %X %X %X" 
    temp_sf_mask: .asciz "%s\n"
    temp_pf_mask:.asciz "read: %s\n"
    temphex_pf_mask:.asciz "read: %X\n"

strtol_end_addr: .word 0x0
multiuse_temp_addr: .word 0x0

.text

main:
    @ preallocate 20 bytes for 20 characters + 2*4*1024 bytes for IAS memory (1024 bytes @ 64bits each)
    @ fp will point to start o MemoryMap (IAS memory entry every 8 bytes)
    @ fp - 8191 will point to 1st char of buffer
    push {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ Cannot writeback to SP! Do we even need to care for SP?

    @ Define register names:
    ac          .req r8
    mq          .req r9
    pc_ias      .req r10
    addr        .req r3
    op1         .req r4
    op1_addr    .req r5
    op2         .req r6
    op2_addr    .req r7

    @ Set FP
    mov fp, sp
    sub fp, fp, #4
    @ Allocate memory on stack
    @ Stack: 1st 8192 bytes: MemoryMap, remaining 20 bytes: string buffer
    sub sp, sp, #8192
    sub sp, sp, #20

    @ Bloco de leitura
    bl build_memory_map
    mov addr, #0
    bl print_memory_map

    @ Bloco de execucao
    ldr r0, =text_sim_started   @ "A simulacao ta comecando."
    bl printf
    
    bl exec_mem_map_begin

    ldr r0, =text_sim_finished  @ "A simulacao terminou."
    bl printf


    pop {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ If SP needs to be back to original location, add:
    add sp, sp, #20
    add sp, sp, #8192

exit:
    mov r0, #0
    mov r7, #1
    svc 0x00
    



@ read_line -->
read_line:
    push {lr}

    ldr r0, =text_scanf_mask
    @ set starting addr on lower stack addresses, since scanf goes up the stack
    mov r1, fp
    sub r1, r1, #8192
    add r1, r1, #1

    bl scanf    @ r0 contains return value. If 0, nothing was read
    @ldr r0, =temp_pf_mask
    @mov r1, fp
    @sub r1, r1, #8192
    @add r1, r1, #1
    @bl printf
    pop {lr}
    bx lr
@ <-- read_line

@ read_hex_input -->
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
    push {addr}                 @ save addr, since r3 can be messed with
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
    pop {addr}                  @ Restore addr
    pop {lr}
    bx lr
@ <-- read_hex_input

@ build_memory_map -->
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
    push {lr}

    bl initialize_zeros     @ Initializes IAS memory with zeros
build_memory_map_loop:
    @ Loop through input while read_line does not return 0:
    bl read_line
    cmp r0, #0
    beq build_memory_map_finish

    bl read_hex_input
    bl add_to_memory @ uses addr, op1 , op1 _add, op2 , op2 addr
    b build_memory_map_loop
build_memory_map_finish:
    pop {lr}
    bx lr
@ <-- build_memory_map

@ print_memory_map -->
print_memory_map:
    @ prints from starting address at 'addr' until 0x3FF (IAS memory) is reached
    push {lr}
print_memory_map_loop:

    ldr r0, =0x3FF  @(1023)
    cmp addr, r0

    popgt {lr}      @ If higher than 1023 return
    bxgt lr

    lsl r0, addr, #3
    add r1, fp, r0
    ldr r0, =temphex_pf_mask
    bl printf
    lsl r0, addr, #3
    add r1, fp, r0
    add r1, r1, #4
    ldr r0, =temphex_pf_mask
    bl printf

    b print_memory_map_loop
    



    



@ <-- print_memory_map

@ load_memory_location -->
load_memory_location:
    @ given address at 'addr' register, load memory into r1, r0
    push {lr}
    @ 'addr' register contains address to be accessed
    lsl r0, addr, #3        @ Multiply by 8, since an IAS memory line occurs every 8 bytes
    add r0, r0, #4          @ addr = addr + 4, will be used below to compensate stack ldrd direction
    ldrd r0, r1, [fp, -r0]  @ Loads [r0][r1] from (lower)[fp - offset][fp - offset + 4](higher)
    @ r1 has [000 DD DDD], r0 has [DD DDD 000]
    pop {lr}
    bx lr
@ <-- load_memory_location

@ extract_memory_elements -->
extract_memory_elements:
    @ 'load_memory_location' must be called before execution 'extract_memory_elements'!
    @ Shitf and set registers, r1 has [000 DD DDD], r0 has [DD DDD 000] from 'load_memory_location':
    @ IAS instruction schema: ---------> [OP1 DAT] ------- [OP2 DAT]
    push {lr}

    and op1, r1, #0xFF000
    and op2, r0, #0xFF000000
    ldr r2, =0x00FFF
    and op1_addr, r1, r2
    ldr r2, =0x00FFF000
    and op2_addr, r0, r2

    pop {lr}
    bx lr
@ <-- extract_memory_elements

@ add_to_memory -->
add_to_memory:
    @ uses addr, op1 , op1 _add, op2 , op2 addr from 'read_hex_input'
    @ uses strd 
    @ lsl addr, addr, #20 addr unused
    @
    @ Converts 40 bits read from input into 2 registers, as defined above, then
    @ inserts into stack at fp+20+2*_addr
    push {lr}

    mov r0, addr                @ Move addr for validation

    bl test_addr
    cmp r0, #1      @ if (error == true)
    beq exit

    lsl op1, op1, #12
    lsl op2, op2, #24
    lsl op2_addr, op2_addr, #12
    orr r1, op1, op1_addr       @ r1 has [000 DD DDD] => [OP1 DAT]
    orr r0, op2, op2_addr       @ r0 has [DD DDD 000] => [OP2 DAT]
    lsl addr, addr, #3          @ Multiply by 8, since an IAS memory line occurs every 8 bytes
    add addr, addr, #4          @ addr = addr + 4, will be used below to compensate stack strd direction
    strd r0, r1, [fp, -addr]    @ Stores [r0][r1] to (lower)[fp - offset][fp - offset + 4](higher)
    @ Description of storage format:
    @   input data: [000 DD DDD][DD DDD 000] IAS memory (40bits SEX to 64bits, centered)
    @   stack insertion:
    @       fp (higher addr) ->: [000 DD DDD]
    @       fp - 4 ----------->: [DD DDD 000]
    @       fp - 8 ----------->: [000 DD DDD]
    @       fp - 12 ---------->: [DD DDD 000]
    @       (...so on for lower addresses...)
    @
    pop {lr}
    bx lr
@ <-- add_to_memory

@ initialize_zeros -->
initialize_zeros:
    @ Initializes 1024*2*4 bytes of memory with zeros
    push {lr}
    mov addr, #0
    mov r4, #0
    mov r5, #0
    ldr r1, =0x3FF      @(1023)
initialize_zeros_compare:
    cmp addr, r1        @ if r0-1023 <= 0 continue initilaizing zeros
    ble initialize_zeros_init
    pop {lr}
    bx lr
initialize_zeros_init:
    strd r4, r5, [fp, -addr]
    add addr, addr, #8  @ New IAS memory line every 8 bytes
    b initialize_zeros_compare
@ <-- initialize_zeros

@ test_addr -->
test_addr:
    @ Tests if address at r0 is less then 1023 (0x3FF)
    push {lr}
    @ r0 contains address to be tested
    ldr r1, =0x3FF  @(1023)
    cmp r0, r1      @ if r0-1023 <= 0 the address is valid
    mov r0, #0      @ return 0 (error = false)
    ble test_addr_exit

    ldr r1, =multiuse_temp_addr
    str r0, [r1]
    ldr r0, =text_invalid_addr
    bl printf
    mov r0, #1      @ return 1 (error = true)
    @mov r0, #1  @ Exit with return code 1
    @mov r7, #1
    @svc 0x00
test_addr_exit:
    pop {lr}
    bx lr
@ <-- test_addr







@ Bloco de execucao do mapa de memoria
exec_mem_map_begin:
    push {r4, r5, r6, lr}

    @ Inicializa variaveis que serao utilizadas
    mov ac, #0      @ r8:ac
    mov mq, #0      @ r9:mq
    mov pc_ias, #0  @ r10:pc_ias
    mov r0, #0      @ r0:inst
    mov r1, #0      @ r1:side
    mov r2, #0      @ r2:jump
    mov addr, #0    @ r3:addr
    mov r4, #0      @ r4:error
    mov r5, #0      @ r5:aux
    mov r6, #0      @ r6:j(iterador)

    push {r0, r1, r2, r3}

    ldr r0, =text_init_state    @ "Estaco inicial"
    bl printf

    ldr r0, =text_reg_info      @ "AC/MQ/PC"
    bl printf

    ldr r0, =text_separator     @ "--------------"
    bl printf

    pop {r0, r1, r2, r3}

    @@ Overview do loop:
    @@ | while(true):
    @@ |  |-> for(j=left, (conditions), j++):
    @@ |  |   (conditions) := side:(left,right) && jump:false && error:false
    @@ |      |-> switch(OP_CODE): seleciona operacao
    @@ |      |-> executa ate encontrar operacao invalida

exec_while_begin:
    mov r6, r1      @ r6:j := r1:side
exec_for_begin:
    push {r0, r1, r2, r3}
    ldr r0, =text_executing_at_addr     @ "Executando instrucao no endereco"
    bl printf
    pop {r0, r1, r2, r3}

    @ Identificando o lado atual a ser executado
    cmp r6, #0
    ldr r5, =0xFFFFF
    sub r0, fp, pc_ias, lsl #3

    ldreq r0, [r0]          @   if (side == left):
    andeq r0, r0, r5        @       r0:inst := memory[pc] (left)
    ldreq r0, =text_intruction_at_left

    subne r0, r0, #4        @   else
    ldrne r0, [r0]          @       r0:inst := memory[pc] (right)
    movne r0, r0, lsr #12
    andne r0, r0, r5
    ldrne r0, =text_intruction_at_right

    push {r0, r1, r2, r3}
    bl printf               @ "instrucao a esquerda/direita"
    pop {r0, r1, r2, r3}

    @ Separando instrucao de endereco
    ldr r5, =0x00FFF
    and addr, r0, r5        @   addr := inst & 0x00FFF
    mov r0, r0, lsr #12     @   inst := (inst >> 12)

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

    @ Default: Erro de instrucao invalida
@ !!! deveria ser fprintf(stderr, string, opcode)
    ldr r0, =text_OP_invalid    @ "Erro!. Instrucao invalida"
    bl printf

    mov r4, #1  @ error := true


op_case_end:
    @ Verifica se houve erro na execucao e encerra caso positivo
    cmp r4, #1             @ r4:error
    beq exec_mem_map_end   @ if (error): return
    
    @ Montagem dos registros AC/MQ/PC para impressao
@ !!! Por enquanto, usando 32-bits mesmo, entao nao fazemos nada aqui

    push {r0, r1, r2, r3}

@   printf("+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n", ac, mq, pc);
    ldr r0, =text_curr_location
    bl printf

@   printf("--------------------------------------------------------------\n");
    ldr r0, =text_separator
    bl printf

    pop {r0, r1, r2, r3}

    @ Loop update
    add r5, r5, #1         @ r1:side (j++)

    @ Loop conditions ((j < 2) && (jump == false))
    cmp r5, #1             @ r5:j
    bgt exec_for_end
    cmp r2, #1             @ r2:jump
    beq exec_for_end

@!!! Acho que nao precisa \/\/\/
@@    cmp r4, #1             @ r4:error == false
@@    beq exec_mem_map_end   @ if (error): return

    b exec_for_begin

exec_for_end:
    @ Memory map execution loop update 
    cmp r2, #0                  @   if (r2:jump == false):
    addeq pc_ias, pc_ias, #1    @       pc := pc + 1
    moveq r1, #0                @       r1:side := left

    movne r2, #0                @   else: r2:jump := false

    b exec_while_begin


exec_mem_map_end:
    pop {r4, r5, r6, lr}
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

