
#include <iostream>
#include <fstream>
#include <sstream>
//Pesquisar sobre gerenciamento de memoria em cpp

//Tipos de elementos
typedef enum{
	Label,
	Instruction,
	Directive
}ElementType;


//Tipos de operacao
typedef enum{
	Load_MQ,
	Load_MQ_MX,
	Stor_MX,
	Load_MX,
	Load_MX_neg,
	Load_MX_mod,
	Jump_M_L,
	Jump_M_R,
	Jump_M_cond_L,
	Jump_M_cond_R,
	Add_MX,
	Add_MX_mod,
	Sub_MX,
	Sub_MX_mod,
	Mul_MX,
	Div_MX,
	Lsh,
	Rsh,
	Stor_M_L,
	Stor_M_R
}OpCodeType;

//Tipos de diretivas
typedef enum{
	Org,
	Align,
	Wfill,
	Word,
	Set
}DirectiveType;

//Elemento
class Element{
	ElementType type;
	string content;
	//Algo mais aqui?
};

//Construtor para elemento do tipo Label
Element::Element(string _content){
	//Campos de Label
	type = Label;
	content = _content;
}

//Construtor para elemento do tipo Diretiva
Element::Element(ElementType _element, DirectiveType _dir){
	type = Directive;
	Directive dir = _dir;
	content = _content;
}

//Construtor para elemento tipo Operacao
Element::Element(ElementType _element, OpCodeType _op){
	type = Instruction;
	opcode = _op;
	content = _content;
}

//Main para testes
int main(){
	Element lab();
	Element dir();
	Element op();
	cout 	<< "Label: " << lab.content << endl 
			<< "Diretiva: " << dir.content << endl 
			<< "Operacao: " << op.content << endl;
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
