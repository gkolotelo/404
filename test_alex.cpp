
#include <iostream>
#include <fstream>
#include <sstream>


//Tipos de elementos
typedef enum ElementType{
	label,
	instruction,
	directive
}ElementType;

//Tipos de operacao
typedef enum OpCodeType{
	
}OpCodeType;

//Tipos de diretivas
typedef enum DirectiveType{
	
}DirectiveType;

//Elemento
class Element{
	ElementType type;
	string content;
	
};

Element::Element(ElementType _element){
	type = label;
}

Element::Element(ElementType _element, DirectiveType _dir){
	Directive dir
	sdadsa

}

Element::Element(ElementType _element, OpCodeType _op){
	
}


int main(){
	Element elem();
	cout << ;
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
