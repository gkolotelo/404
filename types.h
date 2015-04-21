#define types_cpp


#define LD_MQ           "ldmq"
#define LD_MQ_CODE      "0A"

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
#define JMP_M_L_CODE    "0D"
#define JMP_M_R_CODE    "0E"

#define JMP_M_P         "jgez"
#define JMP_M_P_L_CODE  "0F"
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
#define MUL_M_CODE      "0B"

#define DIV_M           "div"
#define DIV_M_CODE      "0C"

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
typedef string InstructionContentContainer;

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

// Elemento
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
    
    //union content {
    DirectiveContentContainer DCC;
    InstructionContentContainer ICC;
    LabelContentContainer LCC;
    WordContentContainer WCC;
    //}C;

    DirectiveType dir;
    OpCodeType opcode;
};

typedef enum{
    Left,
    Right
} Side;

typedef struct{
    string name;
    int addr;
}AddressElement;

typedef struct {
    Element *elem;
    int addr;
    Side side;
    AddressElement* addrLink;
} MemoryElement;
