// types.cpp
//
//
//
//

#define types_cpp


#define LD_MQ 			"LDMQ"
#define LD_MQ_CODE		"0A"

#define LD_MQ_MX 		"LDMQM"
#define LD_MQ_MX_CODE 	"09"

#define ST_MX			"STR"
#define ST_MX_CODE		"21"

#define LD_MX			"LOAD"
#define LD_MX_CODE		"01"

#define LD_MX_NEG		"LDN"
#define LD_MX_NEG_CODE	"02"

#define LD_MX_ABS		"LDABS"
#define LD_MX_ABS_CODE	"03"

#define JMP_M 			"JMP"
#define JMP_M_L_CODE 	"0D"
#define JMP_M_R_CODE 	"0E"

#define JMP_M_P 		"JGEZ"
#define JMP_M_P_L_CODE 	"0F"
#define JMP_M_P_L_CODE 	"10"

#define ADD_MX 			"ADD"
#define ADD_MX_CODE		"05"

#define ADD_MX_ABS 		"ADDABS"
#define ADD_MX_ABS_CODE	"07"

#define SUB_MX 			"SUB"
#define SUB_MX_CODE		"06"

#define SUB_MX_ABS 		"SUBABS"
#define SUB_MX_ABS_CODE	"08"

#define MUL_MX 			"MUL"
#define MUL_MX_CODE		"0B"

#define DIV_MX 			"DIV"
#define DIV_MX_CODE		"0C"

#define LSH 			"LSH"
#define LSH_CODE 		"14"

#define RSH 			"RSH"
#define RSH_CODE 		"15"

#define ST_M 			"STM"
#define ST_M_L_CODE		"12"
#define ST_M_R_CODE		"13"


#include <string>

using namespace std;

// Token Container
typedef struct  {
    string tokens[5];
    int amount;
}TokenContainer;

// Directive Content Container, reune 1 ou 2 dos argumentos de uma diretiva
typedef struct {
    string content1;
    string content2;
    int amount;
}DirectiveContentContainer;

// Instruction Contentn Container, armazena o argumento de uma instrução
typedef string InstructionContentContainer; // Must handle labels

// Label Content Container, armazena o argumento de um label
typedef string LabelContentContainer;

// Word Content Container, armazena o argumento de um half_of_word
typedef string WordContentContainer;

// Tipos de elementos
typedef enum {
    Label,
    Instruction,
    Directive,
    WordElement
}ElementType;


// Tipos de operacao
typedef enum {
    Load_MQ,
    Load_MQ_MX,
    Stor_MX,
    Load_MX,
    Load_MX_neg,
    Load_MX_abs,
    Jump_M,
    // Jump_M_L,
    // Jump_M_R,
    Jump_M_P,  // P por causa do plus
    // Jump_M_cond_L,
    // Jump_M_cond_R,
    Add_MX,
    Add_MX_abs,  // Add_MX_mod
    Sub_MX,
    Sub_MX_abs,  // Sub_MX_mod
    Mul_MX,
    Div_MX,
    Lsh,
    Rsh,
    Stor_M,
    // Stor_M_L,
    // Stor_M_R
    Null
}OpCodeType;

// Tipos de diretivas
typedef enum {
    Org,
    Align,
    Wfill,
    Word,
    Set
}DirectiveType;

// Elemento
class Element {
 public:
    // Constructors:
    Element();
    Element(LabelContentContainer _content);
    Element(DirectiveType _dir, DirectiveContentContainer _content);
    Element(OpCodeType _op, InstructionContentContainer _content);
    // Methods
    string GetOpCodeString();
    // Get, Set Methods:
    ElementType GetElementType() const { return type; }
    DirectiveContentContainer GetDirectiveContentContainer() const { return DCC; }
    InstructionContentContainer GetInstructionContentContainer() const { return ICC; } 
    LabelContentContainer GetLabelContentContainer() const { return LCC; }
    WordContentContainer GetWordContentContainer() const { return WCC; }
    DirectiveType GetDirectiveType() const { return dir; }
    OpCodeType GetOpCodeType() const { return opcode; }
    WordContentContainer SetWordContentContainer(WordContentContainer _wcc) { WCC = _wcc; }

 private:
    ElementType type;
    
    //union content {
    DirectiveContentContainer DCC;
    InstructionContentContainer ICC;
    LabelContentContainer LCC;
    WordContentContainer WCC;
    //}C;

    DirectiveType dir;
    OpCodeType opcode;
};

// Construtor de um elemento nulo
Element::Element() {
    type = WordElement;
    WCC = "00000";
}

// Construtor para elemento do tipo Label
Element::Element(LabelContentContainer _content) {
    // Campos de Label
    type = Label;
    LCC = _content;
}

// Construtor para elemento do tipo Diretiva
Element::Element(DirectiveType _dir, DirectiveContentContainer _content) {
    type = Directive;
    dir = _dir;
    DCC = _content;
}

// Construtor para elemento tipo Operacao
Element::Element(OpCodeType _op, InstructionContentContainer _content) {
    type = Instruction;
    opcode = _op;
    if (_content == "")
        ICC = "000";
    else
        ICC = _content;
}

string Element::GetOpCodeString() {
    // Temporary:
    return "FF";
}
