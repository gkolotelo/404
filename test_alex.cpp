
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.cpp"
//#include "reading.cpp"

//Main para testes
//int main(){
//	Element lab("ThisIsALabel");
//	Element op(Sub_MX, "ThisIsAOperation");
//	Element dir(Wfill,"ThisIsADirective");
//	
//	cout 	<< "Label: " << lab.content << " | tipo: " << lab.type << endl 
//			<< "Operacao: " << op.content << " | tipo: " << op.type << " | " << op.opcode << endl
//			<< "Diretiva: " << dir.content << " | tipo: " << dir.type << " | " << dir.dir << endl;
//}

typedef enum{
	Left,
	Right
}Side;

class MemoryMap{
	Element elem;
	int addr;
	Side side;
	tipo label_link;

};

//Tem que pensar como vai ser isso
//Acho q ao inves de ser uma funcao sozinha, 
//vai ser uma etapa da main ou de outra funcao.
void add(){
	switch(elem.type){
		case Label:
		case Instruction:
		case Directive:
	}
}




//void montagem(){
//	while(leitura(arquivo, linha)){
//		for each elemento in linha{ //acho q precisa de um interpretador
//			switch(elemento.type){
//				case label{
//
//				}
//				case directive{
//
//				}
//				case operation{
//
//				}
//			}
//		}
//	}
//}
//
//void escrita(memorymap){
//
//}