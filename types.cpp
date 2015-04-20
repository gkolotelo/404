// types.cpp
//
//
//
//

#define types_cpp

#include <string>

using namespace std;

// Token Container
typedef struct  {
    string tokens[5];
    int amount;
}TokenContainer;

// Directive Content Container, reune 1 ou 2 dos argumentos de uma diretiva
typedef struct {
    int64_t content1;
    int64_t content2;
    int amount;
}DirectiveContentContainer;

// Instruction Contentn Container, armazena o argumento de uma instrução
typedef int InstructionContentContainer;

// Label Content Container, armazena o argumento de um label
typedef string LabelContentContainer;

// Tipos de elementos
typedef enum {
    Label,
    Instruction,
    Directive
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
    Element(LabelContentContainer _content);
    Element(DirectiveType _dir, DirectiveContentContainer _content);
    Element(OpCodeType _op, InstructionContentContainer _content);
    // Get, Set Methods:
    ElementType GetElementType() const { return type; }
    DirectiveContentContainer GetDirectiveContentContainer() const { return DCC; }
    InstructionContentContainer GetInstructionContentContainer() const { return ICC; }
    LabelContentContainer GetLabelContentContainer() const { return LCC; }
    DirectiveType GetDirectiveType() const { return dir; }
    OpCodeType GetOpCodeType() const { return opcode; }

 private:
    ElementType type;
    
    //union content {
    DirectiveContentContainer DCC;
    InstructionContentContainer ICC;
    LabelContentContainer LCC;
    //}C;

    DirectiveType dir;
    OpCodeType opcode;
};

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
    ICC = _content;
}
