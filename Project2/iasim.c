#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

enum {
	OP_LOAD = 0x01,	//OK
	OP_LOADMQM = 0x09,			//OK
	OP_LOADMQ = 0x0A,			//OK
	OP_LOADABS = 0x03,			//OK
	OP_LOADN = 0x02,			//OK
	OP_STOR = 0x21,				//OK
	OP_STORL = 0x12,			//OK
	OP_STORR = 0x13,			//OK
	OP_ADD = 0x05,				//OK
	OP_ADDABS = 0x07,			//OK
	OP_SUB = 0x06,				//OK
	OP_SUBABS = 0x08,			//OK
	OP_MUL = 0x0B,				//OK
	OP_DIV = 0x0C,				//OK
	OP_RSH = 0x15,				//OK
	OP_LSH = 0x14,				//OK
	OP_JUMPL = 0x0D,			//OK
	OP_JUMPR = 0x0E,			//OK
	OP_JUMPPL = 0x0F,			//OK
	OP_JUMPPR = 0x10,			//OK
};

int test_addr(int addr) {
	if (addr < 0 || addr > 1023) {
		fprintf(stderr, "IASIM: Erro! Endereco invalido de numero %04X.\n",
				addr);
		return 1;
	}
	return 0;
}

void simulate(FILE * in) {
	char buffer[1000], *ptr;
	int64_t memory[1024] = { 0 };
	int64_t ac, mq, pc;
	int i, j, addr, inst, jump, first, error;

	while (fgets(buffer, 1000, in) != NULL) {
		if (sscanf(buffer, "%X %n", &addr, &i) > 0) {
			if (test_addr(addr) == 1) {
				return;
			}
			ptr = buffer + i;
			i = 0;
			while (i < 10) {
				if (isxdigit(*ptr)) {
					memory[addr] = memory[addr] << 4;
					if (*ptr >= 'A') {
						memory[addr] |= (10 + (toupper(*ptr) - 'A'));
					} else {
						memory[addr] |= (*ptr - '0');
					}
					i++;
				}
				ptr++;
			}
		}
	}

	ac = 0;
	mq = 0;
	pc = 0;
	jump = 0;
	first = 0;
	error = 0;
	
	printf("@ Estado inicial:\n");
	printf("+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n",
			ac & 0xFFFFFFFFFF, mq & 0xFFFFFFFFFF, pc & 0xFFFFFFFFFF);
	printf("--------------------------------------------------------------\n");	
	
	while (1) {
		for (j = first; j < 2 && jump == 0 && error == 0; j++) {
			printf("@ Executando instrucao no endereco %010X ",
					(unsigned int)pc);
			if (j == 0) {
				inst = (memory[pc] >> 20) & 0xFFFFF;
				printf("(instrucao a esquerda)\n");
			} else {
				inst = memory[pc] & 0xFFFFF;
				printf("(instrucao a direita)\n");
			}
			addr = inst & 0x0FFF;
			switch (inst >> 12) {
				case OP_LOAD:
					printf("@ LOAD M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						ac = memory[addr];
					}
					break;
				case OP_LOADMQM:
					printf("@ LOAD MQ,M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						mq = memory[addr];
					}
					break;
				case OP_LOADMQ:
					printf("@ LOAD MQ, X = 0x%04X\n", addr);
					error = test_addr(addr);
					ac = mq;
					break;
				case OP_LOADABS:
					printf("@ LOAD |(M(X)|, X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						if (memory[addr] >> 39 != 0) {
							ac = -memory[addr];
						} else {
							ac = memory[addr];
						}
					}
					break;
				case OP_LOADN:
					printf("@ LOAD -(M(X)), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						ac = -(memory[addr]);
					}
					break;
				case OP_STOR:
					printf("@ STOR M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						memory[addr] = ac & 0xFFFFFFFFFF;
					}
					break;
				case OP_STORL:
					printf("@ STOR M(X,8:19), X =0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						memory[addr] &= 0xFF000FFFFF;
						memory[addr] ^= (ac & 0x0FFF) << 20;
					}
					break;
				case OP_STORR:
					printf("@ STOR M(X,28:39), X =0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						memory[addr] &= 0xFFFFFFF000;
						memory[addr] ^= (ac & 0x0FFF);
					}
					break;
				case OP_ADD:
					printf("@ ADD M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						ac += memory[addr];
					}
					break;
				case OP_ADDABS:
					printf("@ ADD |M(X)|, X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						if (memory[addr] >> 39 != 0) {
							ac -= memory[addr];
						} else {
							ac += memory[addr];
						}
					}
					break;
				case OP_SUB:
					printf("@ SUB M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						ac -= memory[addr];
					}
					break;
				case OP_SUBABS:
					printf("@ SUB |M(X)|, X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						if (memory[addr] >> 39 != 0) {
							ac += memory[addr];
						} else {
							ac -= memory[addr];
						}
					}
					break;
				case OP_MUL:{
						printf("@ MUL M(X), X = 0x%04X\n", addr);
						error = test_addr(addr);
						if (!error) {
							int64_t l0, h0, l1, h1;
							l0 = mq & 0xFFFFF;
							h0 = mq >> 20;
							l1 = memory[addr] & 0xFFFFF;
							h1 = memory[addr] >> 20;
							mq = ((l0 * l1) + ((l0 * h1 +
									l1 * h0) << 20)) & 0xFFFFFFFFFF;
							if (mq < (l0 * l1) & 0xFFFFFFFF) {
								ac = 1 + (h0 * h1) + ((l0 * h1 + l1 * h0) >> 20);
							} else {
								ac = (h0 * h1) + ((l0 * h1 + l1 * h0) >> 20);
							}
						}
					}
					break;
				case OP_DIV:
					printf("@ DIV M(X), X = 0x%04X\n", addr);
					error = test_addr(addr);
					if (!error) {
						if (memory[addr] == 0) {
							fprintf(stderr, "IASIM: Erro! Divisao por zero.\n");
							error = 1;
						} else {
							int64_t div, op;
							div = memory[addr];
							if (memory[addr] >> 39 != 0) {
								div = -div;
							}
							op = ac;
							if (ac >> 39 != 0) {
								op = -op;
							}
							mq = op / div;
							ac = op % div;
						}
					}
					break;
				case OP_RSH:
					printf("@ RSH, X = 0x%04X\n", addr);
					ac = ac >> 1;
					break;
				case OP_LSH:
					printf("@ LSH, X = 0x%04X\n", addr);
					ac = ac << 1;
					break;
				case OP_JUMPL:
					printf("@ JUMP M(X,0:19), X = 0x%04X\n", addr);
					error = test_addr(addr);
					pc = addr;
					first = 0;
					jump = 1;
					break;
				case OP_JUMPR:
					printf("@ JUMP M(X,20:39), X = 0x%04X\n", addr);
					error = test_addr(addr);
					pc = addr;
					first = 1;
					jump = 1;
					break;
				case OP_JUMPPL:
					printf("@ JUMP+ M(X,0:19), X = 0x%04X\n", addr);
					if (ac >> 39 == 0) {
						error = test_addr(addr);
						pc = addr;
						first = 0;
						jump = 1;
						printf("@ Salto realizado\n");
					}
					break;
				case OP_JUMPPR:
					printf("@ JUMP+ M(X,20:39), X = 0x%04X\n", addr);
					if (ac >> 39 == 0) {
						error = test_addr(addr);
						pc = addr;
						first = 1;
						jump = 1;
						printf("@ Salto realizado\n");
					}
					break;
				default:
					fprintf(stderr,
							"IASIM: Erro! Instrucao invalida com opcode %02X.\n",
							inst >> 12);
					error = 1;
					break;
			}
			if (error == 1) {
				return;
			} else {
				ac &= 0xFFFFFFFFFF;
				mq &= 0xFFFFFFFFFF;
				pc &= 0xFFFFFFFFFF;
			}
			printf("+ AC:  0x%010llX     MQ: 0x%010llX        PC: 0x%010llX\n", ac, mq, pc);
			printf("--------------------------------------------------------------\n");
		}
		if (jump == 0) {
			pc = pc + 1;
			first = 0;
		} else {
			jump = 0;
		}
	}
}

int main(int argc, char *argv[]) {
	FILE *in;
	/* Checagem da existência e permissões dos arquivos passados como parâmetros */
	if ((in = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "IASIM: Erro! Nao foi possivel abrir arquivo.\n");
		exit(1);
	}
	printf("\nIASIM: A simulacao esta comecando.\n\n");
	simulate(in);
	printf("\nIASIM: A simulacao terminou.\n\n");
	fclose(in);
	return 0;
}
