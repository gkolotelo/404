
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.cpp"
//#include "reading.cpp"

//Main para testes
int main(){
	Element lab("ThisIsALabel");
	Element op(Load_MQ, "ThisIsAOperation");
	Element dir(Org,"ThisIsADirective");
	
	cout 	<< "Label: " << lab.content << " | tipo: " << lab.type << endl 
			<< "Operacao: " << op.content << " | tipo: " << op.type << " | " << op.opcode << endl
			<< "Diretiva: " << dir.content << " | tipo: " << dir.type << " | " << dir.dir << endl;
}

//int interpreter();

//void assembly(){
//	//Loop de leitura
//	while(){
//		//Loop de traducao
//		while(){
//
//		}
//	}
//}
