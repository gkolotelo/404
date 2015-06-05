#define TYPES_H

/* ------ Lista de mnemonicos ------ */
#define LD_MQ           "ldmq"
#define LD_MQ_CODE      "0a"

#define LD_MQ_M         "ldmqm"
#define LD_MQ_M_CODE    "09"

#define ST_R            "str"
#define ST_R_CODE       "21"

#define LD_M            "load"
#define LD_M_CODE       "01"

#define LD_M_NEG        "ldn"
#define LD_M_NEG_CODE   "02"

#define LD_M_ABS        "ldabs"
#define LD_M_ABS_CODE   "03"

#define JMP_M           "jmp"
#define JMP_M_L_CODE    "0d"
#define JMP_M_R_CODE    "0e"

#define JMP_M_P         "jgez"
#define JMP_M_P_L_CODE  "0f"
#define JMP_M_P_R_CODE  "10"

#define ADD_M           "add"
#define ADD_M_CODE      "05"

#define ADD_M_ABS       "addabs"
#define ADD_M_ABS_CODE  "07"

#define SUB_M           "sub"
#define SUB_M_CODE      "06"

#define SUB_M_ABS       "subabs"
#define SUB_M_ABS_CODE  "08"

#define MUL_M           "mul"
#define MUL_M_CODE      "0b"

#define DIV_M           "div"
#define DIV_M_CODE      "0c"

#define LSH             "lsh"
#define LSH_CODE        "14"

#define RSH             "rsh"
#define RSH_CODE        "15"

#define ST_M            "stm"
#define ST_M_L_CODE     "12"
#define ST_M_R_CODE     "13"


#include <string>

using namespace std;

// Token Container
// Armazena informações sobre a string de entrada, quebrada em tokens
typedef struct  {
    string tokens[5];
    int amount;
}TokenContainer;

// Directive Content Container, reune 1 ou 2 dos argumentos de uma diretiva
// Armazena informações sobre uma diretiva
typedef struct {
    string content1;
    string content2;
    int amount;
}DirectiveContentContainer;

// Instruction Content Container
// Armazena argumento de uma instrução
typedef string InstructionContentContainer;

// Label Content Container
// Armazena nome de uma label
typedef string LabelContentContainer;

// Word Content Container
// Armazena o argumento de um half_of_word (20bits)
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
    Jump_M_P,
    Add_MX,
    Add_MX_abs,
    Sub_MX,
    Sub_MX_abs,
    Mul_MX,
    Div_MX,
    Lshift,
    Rshift,
    Stor_M,
}OpCodeType;

// Tipos de diretivas
typedef enum {
    Org,
    Align,
    Wfill,
    Word,
    Set
}DirectiveType;

// Lado do mapa de memória
typedef enum{
    Left,
    Right
} Side;

// Elemento
// Armazena informações sobre uma diretiva, instrução, Word ou label que
// será usado para processamento do mapa de memória e inserção no mesmo
class Element {
 public:
    // Constructors:
    Element();
    Element(LabelContentContainer _content);
    Element(DirectiveType _dir, DirectiveContentContainer _content);
    Element(OpCodeType _op, InstructionContentContainer _content);
    // Methods
    string GetOpCodeString(Side side);
    // Get, Set Methods:
    ElementType GetElementType() const { return type; }
    DirectiveContentContainer GetDirectiveContentContainer() const { return DCC; }
    InstructionContentContainer GetInstructionContentContainer() const { return ICC; } 
    LabelContentContainer GetLabelContentContainer() const { return LCC; }
    WordContentContainer GetWordContentContainer() const { return WCC; }
    DirectiveType GetDirectiveType() const { return dir; }
    OpCodeType GetOpCodeType() const { return opcode; }
    void SetWordContentContainer(WordContentContainer _wcc) { WCC = _wcc; }
    void SetInstructionContentContainer(InstructionContentContainer _icc) { ICC = _icc; }

 private:
    ElementType type;
    
    DirectiveContentContainer DCC;
    InstructionContentContainer ICC;
    LabelContentContainer LCC;
    WordContentContainer WCC;
    
    DirectiveType dir;
    OpCodeType opcode;
};

typedef struct{
    string name;
    int addr;
    Side side;
}AddressElement;

typedef struct {
    Element *elem;
    int addr;
    Side side;
    AddressElement* addrLink;
} MemoryElement;
