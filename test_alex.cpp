
#include <iostream>
#include <fstream>
#include <sstream>
#include "types.cpp"
//#include "reading.cpp"

//Main para testes
int main(){
	Element lab("ThisIsALabel");
	Element op(Sub_MX, "ThisIsAOperation");
	Element dir(Wfill,"ThisIsADirective");
	
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
