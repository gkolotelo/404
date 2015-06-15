@ +----------------------------------------+
@ | MC404 Turma E - 2015 semestre 1        |
@ | Trabalho 2 - Simulador de IAS para ARM |
@ +----------------------------------------+
@ | 135964 Guilherme Kairalla Kolotelo     |
@ | 137943 Alexandre Seidy Ioshisaqui      |
@ +----------------------------------------+

.globl main

.extern printf
.extern scanf
.extern strtol
.extern fgets
.extern getchar

@ Define register names:
ac          .req r8
mq          .req r9
pc_ias      .req r10
addr        .req r3
op1         .req r4
op1_addr    .req r5
op2         .req r6
op2_addr    .req r7

@ Define aliases:
.equ alloc_space, 8212          @ Stack: 1st 8192 bytes: MemoryMap, remaining 20 bytes: string buffer
.equ buffer_pointer, 8208       @ Points to where buffer starts

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
    temp_sf_mask: .asciz "%s\n"
    temp_pf_mask:.asciz "read: %s\n"
    temphex_pf_mask:.asciz "%X\n"
    temphex_pf_mask_nonl:.asciz "%X "

strtol_end_addr: .word 0x0

.text

main:
    @ preallocate 20 bytes for 20 characters + 2*4*1024 bytes for IAS memory (1024 bytes @ 64bits each)
    @ fp will point to start o MemoryMap (IAS memory entry every 8 bytes)
    @ fp - 8191 will point to 1st char of buffer
    push {r4,r5,r6,r7,r8,r9,r10,fp,lr}
    @ Cannot writeback to SP! Do we even need to care for SP?


    @ Set FP
    mov fp, sp
    sub fp, fp, #4
    @ Allocate memory on stack
    ldr r0, =alloc_space
    sub sp, sp, r0

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
    ldr r0, =alloc_space
    add sp, sp, r0

exit:
    mov r0, #0
    mov r7, #1
    svc 0x00
    



@ read_line -->
read_line:
    push {lr}

    @ Older scanf mode
    ldr r0, =text_scanf_mask
    @ set starting addr on lower stack addresses, since scanf goes up the stack
    @mov r1, fp
    ldr r1, =buffer_pointer 
    sub r1, fp, r1
    @add r1, r1, #1+4 dont thin I need
    
    bl scanf    @ r0 contains return value. If 0, nothing was read
    push {r0}
    bl getchar  @ gets remaining newline from stdin
    pop {r0}

    @@ Newer fgets mode:
    @mov r0, fp          @ r0 has pointer to buffer
    @sub r0, r0, #8192
    @add r0, r0, #1
    @mov r1, #19         @ max 20 characters
    @mov r2, #0          @ stdin
    @bl fgets

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
    @mov r0, fp     
    ldr r1, =buffer_pointer 
    sub r0, fp, r1              @ (strtol arg) str: fp-8191, points to 1st char read by scanf
    @add r0, r0, #1              @ must add 1, since immediate is higher than 12bits (-8192+1)
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
    sub r1, fp, r0
    ldr r1, [r1]
    ldr r0, =temphex_pf_mask_nonl
    push {addr}
    bl printf
    pop {addr}
    lsl r0, addr, #3
    sub r1, fp, r0
    sub r1, r1, #4
    ldr r1, [r1]
    lsr r1, r1, #12
    ldr r0, =temphex_pf_mask
    push {addr}
    bl printf
    pop {addr}

    add addr, addr, #1
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
    ldr r1, =8192      @(1023*2*4)
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
    movle r0, #0      @ return 0 (error = false)
    ble test_addr_exit

    mov r1, r0
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


@ load_mem_map_word -->
@ Carrega uma palavra em 32-bits
@ input:    r0:address
@ output:   r0:word (32-bits)
load_mem_map_word:
    push {lr}

    bl load_memory_location     @ input:addr
                                @ output:r1(left), r0(right)
    ldr r2, =0x00000FFF         @ left:     000LLLLL
    and r1, r1, r2              @        => 00000LLL
    mov r1, r1, lsl #20         @        => LLL00000
                                @ right:    RRRRR000
    mov r0, r0, lsr #12         @        => 000RRRRR

    add r0, r0, r1              @ word:     LLLRRRRR

    pop {lr}
    bx lr
@ <-- load_mem_map_word

@@ Bloco de execucao do mapa de memoria

@ exec_mem_map -->
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
    mov r1, #0
    mov r2, #0
    mov r3, #0
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
@ !!! deveria ser (unsigned int)pc
    mov r1, pc_ias
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
    mov r0, r0, lsr #12     @   r0:inst := (inst >> 12)

    @ switch(OP_CODE)
    cmp r0, #0x01  @ case LOAD:
    beq op_load

    cmp r0, #0x09  @ case LOADMQM:
    beq op_loadmqm

    cmp r0, #0x0A  @ case LOADMQ:
    beq op_loadmq

    cmp r0, #0x03  @ case LOADABS:
    beq op_loadabs

    cmp r0, #0x02  @ case LOADN:
    beq op_loadn

    cmp r0, #0x21  @ case STOR:
    beq op_stor

    cmp r0, #0x12  @ case STORL:
    beq op_storl

    cmp r0, #0x13  @ case STORR:
    beq op_storr

    cmp r0, #0x05  @ case ADD:
    beq op_add

    cmp r0, #0x07  @ case ADDABS:
    beq op_addabs

    cmp r0, #0x06  @ case SUB:
    beq op_sub

    cmp r0, #0x08  @ case SUBABS:
    beq op_subabs

    cmp r0, #0x0B  @ case MUL:
    beq op_mul

    cmp r0, #0x0C  @ case DIV:
    beq op_div

    cmp r0, #0x15  @ case RSH:
    beq op_rsh

    cmp r0, #0x14  @ case LSH:
    beq op_lsh

    cmp r0, #0x0D  @ case JUMPL:
    beq op_jumpl

    cmp r0, #0x0E  @ case JUMPR:
    beq op_jumpr

    cmp r0, #0x0F  @ case JUMPPL:
    beq op_jumppl

    cmp r0, #0x10  @ case JUMPPR:
    beq op_jumppr

    @ Default: Erro de instrucao invalida
@ !!! deveria ser fprintf(stderr, string, opcode)
    ldr r0, =text_OP_invalid    @ "Erro!. Instrucao invalida"
    mov r1, addr
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
    mov r1, ac
    mov r2, mq
    mov r3, pc_ias
    bl printf

@   printf("--------------------------------------------------------------\n");
    ldr r0, =text_separator
    bl printf

    pop {r0, r1, r2, r3}

    @ Loop update
    add r5, r5, #1         @ r5:j (j++)

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
@ <-- exec_mem_map


@@ Operacoes do IAS
@ op_load -->
op_load:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LOAD   @ "LOAD M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    mov ac, r0            @ ac = memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_load

@ op_loadmqm -->
op_loadmqm:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LOADMQM   @ "LOAD MQ,M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    mov mq, r0            @ mq = memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_loadmqm

@ op_loadmq -->
op_loadmq:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LOADMQ   @ "LOAD MQ(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0    
    movne r4, #1

    mov ac, mq              @ ac = mq

    b op_case_end
@ <-- op_loadmq

@ op_loadabs -->
op_loadabs:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LOADABS   @ "LOAD |M(X)|"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    @mov r1, r0, lsr #39      @ if (memory[addr] >> 39 != 0)
    cmp r1, #0
    moveq ac, r0            @ ac = memory[addr]
    movne r1, #0            @ ac = -memory[addr]
    subne ac, r1, r0
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_loadabs

@ op_loadn -->
op_loadn:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LOADN   @ "LOAD -(M(X))"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    mov r1, #0
    sub ac, r1, r0          @ ac = -memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_loadn

@ op_stor -->
op_stor:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_STOR   @ "STOR M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

@    push {}
    @ STORE 40-bits AQUI
@    pop {}

    b op_case_end
@ <-- op_stor

@ op_storl -->
op_storl:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_STORL   @ "STOR M(X, 8:19)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

@    push {}
    @ STORE LEFT 12-bits AQUI
@    pop {}

    b op_case_end
@ <-- op_storl

@ op_storr -->
op_storr:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_STORR   @ "STOR M(X, 28:39)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

@    push {}
    @ STORE RIGHT 12-bits AQUI
@    pop {}

    b op_case_end
@ <-- op_storr

@ op_add -->
op_add:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_ADD   @ "ADD M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    add ac, ac, r0          @ ac += memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_add

@ op_addabs -->
op_addabs:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_ADDABS   @ "ADD |M(X)|"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    @mov r1, r0, lsr #39      @ if (memory[addr] >> 39 != 0)
    cmp r1, #0
    addeq ac, ac, r0        @ ac += memory[addr]
    subne ac, ac, r0        @ ac -= memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_addabs

@ op_sub -->
op_sub:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_SUB   @ "SUB M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    sub ac, ac, r0          @ ac -= memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_sub

@ op_subabs -->
op_subabs:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_SUBABS   @ "SUB |M(X)|"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    push {r0, r1, r2}
    bl load_mem_map_word
    @mov r1, r0, lsr #39      @ if (memory[addr] >> 39 != 0)
    cmp r1, #0
    subeq ac, ac, r0        @ ac -= memory[addr]
    addne ac, ac, r0        @ ac += memory[addr]
    pop {r0, r1, r2}

    b op_case_end
@ <-- op_subabs

@ op_mul -->
op_mul:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_MUL   @ "MUL M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

    @ Multiplicacao (32-bits)
    @ mq:           AAAABBBB
    @ memory[addr]: CCCCDDDD
    @ h0: AAAA ; l0: BBBB
    @ h1: CCCC ; l1: DDDD
    @ mq = ((l0 * l1) + ((l0 * h1 + l1 * h0) << 20)) & 0xFFFFFFFF
    @ if (mq < (l0 * l1) & 0xFFFFFFFF):
    @       ac = 1 + (h0 * h1) + ((l0 * h1 + l1 * h0) >> 20)
    @ else: ac = (h0 * h1) + ((l0 * h1 + l1 * h0) >> 20)
    push {r0, r1, r2, r3}
    
    @ r0:(l0 * l1)
    @ r1:(l0 * h1)
    @ r2:(l1 * h0)
    @ r3:(h0 * h1)
    
    pop {r0, r1, r2, r3}

    b op_case_end
@ <-- op_mul

@ op_div -->
op_div:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_DIV  @ "DIV M(X)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    bne op_case_end

@    push {}
    @ DIVISAO
@    pop {}

    b op_case_end
@ <-- op_div

@ op_rsh -->
op_rsh:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_RSH  @ "RSH"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    mov ac, ac, lsr #1

    b op_case_end
@ <-- op_rsh

@ op_lsh -->
op_lsh:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_LSH  @ "LSH"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    mov ac, ac, lsl #1

    b op_case_end
@ <-- op_lsh

@ op_jumpl -->
op_jumpl:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_JUMPL  @ "JUMP M(X, 0:19)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    mov pc_ias, addr        @ pc = addr
    mov r1, #0              @ side = left
    mov r2, #1              @ jump = true

    b op_case_end
@ <-- op_jumpl

@ op_jumpr -->
op_jumpr:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_JUMPR  @ "JUMP M(X, 20:39)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    mov pc_ias, addr        @ pc = addr
    mov r1, #1              @ side = right
    mov r2, #1              @ jump = true

    b op_case_end
@ <-- op_jumpr

@ op_jumppl -->
op_jumppl:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_JUMPPL  @ "JUMP+ M(X, 0:19)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0}
    @mov r0, ac, lsr #39
    cmp r0, #0
    pop {r0}
    bne op_case_end

    @ if(ac >> 39 == 0)
    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    mov pc_ias, addr        @ pc = addr
    mov r1, #0              @ side = left
    mov r2, #1              @ jump = true

    b op_case_end
@ <-- op_jumppl

@ op_jumppr -->
op_jumppr:
    push {r0, r1, r2, r3}
    ldr r0, =text_OP_JUMPPr  @ "JUMP+ M(X, 20:39)"
    mov r1, addr
    bl printf
    pop {r0, r1, r2, r3}

    push {r0}
    @mov r0, ac, lsr #39
    cmp r0, #0
    pop {r0}
    bne op_case_end

    @ if(ac >> 39 == 0)
    push {r0, r1, r2, r3}
    mov r0, addr            @ error = test_addr(addr)
    bl test_addr
    cmp r0, #0              @ if(!error)
    pop {r0, r1, r2, r3}

    moveq r4, #0
    movne r4, #1

    mov pc_ias, addr        @ pc = addr
    mov r1, #1              @ side = right
    mov r2, #1              @ jump = true

    b op_case_end
@ <-- op_jumppr
