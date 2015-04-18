// types.cpp
//
//
//
//

#define types_cpp
using namespace std;

//Token Container
typedef struct 
{
	string tokens[5];
	int amount = 0;
}TokenContainer;

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
	public:
		Element(string _content);
		Element(DirectiveType _dir, string _content);
		Element(OpCodeType _op, string _content);
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
Element::Element(DirectiveType _dir, string _content){
	type = Directive;
	DirectiveType dir = _dir;
	content = _content;
}

//Construtor para elemento tipo Operacao
Element::Element(OpCodeType _op, string _content){
	type = Instruction;
	OpCodeType opcode = _op;
	content = _content;
}