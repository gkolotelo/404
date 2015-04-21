// types.cpp
//
//
//
//
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
