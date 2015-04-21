#include "types.h"

#define LD_MQ 			"ldmq"
#define LD_MQ_CODE		"0A"

#define LD_MQ_M 		"ldmqm"
#define LD_MQ_M_CODE 	"09"

#define ST_R			"str"
#define ST_R_CODE		"21"

#define LD_M			"load"
#define LD_M_CODE		"01"

#define LD_M_NEG		"ldn"
#define LD_M_NEG_CODE	"02"

#define LD_M_ABS		"ldabs"
#define LD_M_ABS_CODE	"03"

#define JMP_M 			"jmp"
#define JMP_M_L_CODE 	"0D"
#define JMP_M_R_CODE 	"0E"

#define JMP_M_P 		"jgez"
#define JMP_M_P_L_CODE 	"0F"
#define JMP_M_P_R_CODE 	"10"

#define ADD_M 			"add"
#define ADD_M_CODE		"05"

#define ADD_M_ABS 		"addabs"
#define ADD_M_ABS_CODE	"07"

#define SUB_M 			"sub"
#define SUB_M_CODE		"06"

#define SUB_M_ABS 		"subabs"
#define SUB_M_ABS_CODE	"08"

#define MUL_M 			"mul"
#define MUL_M_CODE		"0B"

#define DIV_M 			"div"
#define DIV_M_CODE		"0C"

#define LSH 			"lsh"
#define LSH_CODE 		"14"

#define RSH 			"rsh"
#define RSH_CODE 		"15"

#define ST_M 			"stm"
#define ST_M_L_CODE		"12"
#define ST_M_R_CODE		"13"


#include <string>

using namespace std;




// Construtor de um elemento nulo
Element::Element() {
    type = WordElement;
    WCC = "00 000";
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

string Element::GetOpCodeString(Side side) {
    // Temporary:
    switch (opcode) {
        case Load_MQ: return LD_MQ_CODE;
        case Load_MQ_MX: return LD_MQ_M_CODE;
        case Stor_MX: return ST_R_CODE;
        case Load_MX: return LD_M_CODE;
        case Load_MX_neg: return LD_M_NEG_CODE;
        case Load_MX_abs: return LD_M_ABS_CODE;
        case Jump_M: {
            if (side == Right) return JMP_M_R_CODE;
            return JMP_M_L_CODE;
        }
        case Jump_M_P: {
            if (side == Right) return JMP_M_P_R_CODE;
            return JMP_M_P_L_CODE;
        }
        case Add_MX: return ADD_M_CODE;
        case Add_MX_abs: return ADD_M_ABS_CODE;
        case Sub_MX: return SUB_M_CODE;
        case Sub_MX_abs: return SUB_M_ABS_CODE;
        case Mul_MX: return MUL_M_CODE;
        case Div_MX: return DIV_M_CODE;
        case Lshift: return LSH_CODE;
        case Rshift: return RSH_CODE;
        case Stor_M: {
            if (side == Right) return ST_M_R_CODE;
            return ST_M_L_CODE;
        }
    }
}
