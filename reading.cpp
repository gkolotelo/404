// reading.cpp
// Methods for input reading and processing
#include "reading.h"
#include "errors.h"
using namespace std;


// Retorna a string removendo os comentarios (delimitados por '#')
string getLineNoCommentLowercase(string in) {
    string tempStr;
    stringstream data;
    data << in;
    getline(data, tempStr, '#');
    transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
    return tempStr;
}

// Retorna o tipo do elemento (rotulo, diretiva ou instrucao)
ElementType getType(string in) {
    if (in.back() == ':')
        return Label;
    if (in.front() == '.')
        return Directive;
    return Instruction;
}


string splitLabel(string *in) {
    int pos = in->find(':');
    if (pos == -1) return "";
    string tmp = in->substr(0, pos + 1);
    *in = in->substr(pos + 1);

    return tmp;
}


void stripM(string *in) {
    if (in->length() == 0) return;
    if (in->front() != 'm')
        ;  // ADD_ERROR must have M() identifier
    in->pop_back();
    in->erase(0,2);
}

// Conversao de valores das bases hexadecimal, binaria e octal para decimal
int64_t convertValue(string inStr) {
    // ADD_ERROR deve detectar valores invalidos, como 0xg4 , 0of4 , dec 45g6 , 0b1021
    char *in = (char *)inStr.c_str();
    if (in[1] == 'x' || in[2] == 'x') return strtol(in + 2, NULL, 16);
    else if (in[1] == 'b' || in[2] == 'b') return strtol(in + 2, NULL, 2);
    else if (in[1] == 'o' || in[2] == 'o') return strtol(in + 2, NULL, 8);
    else return strtol(in, NULL, 10);
}


DirectiveContentContainer contentParser(string token1, string token2) {
    DirectiveContentContainer CC;
    CC.content1 = token1;
    if (token2 == "") {
        CC.amount = 1;
        return CC;
    }
    CC.content2 = token2;
    CC.amount = 2;
    return CC;
}


TokenContainer tokenize(string in) {
    TokenContainer TC;
    TC.amount = 0;
    if (in == "") return TC;

    int i = 0;
    char *token;

    token = strtok((char *)in.c_str(), ", \t");

    while(token != NULL) {
        (TC.tokens[i++]).assign(token);
        token = strtok(NULL, ", \t");
    }
    TC.amount = i;  // Store number of tokens on zero index
    return TC;
}

/* 	A funcao recebe um TokenContainer, interpreta e armazena os
	metadados do token em um Element e o retorna.
*/
Element* getElement(TokenContainer TC) {
    if (TC.amount == 0) return NULL;  // No tokens to get Element from

    if (getType(TC.tokens[0]) == Label) {
        TC.tokens[0].pop_back();
        Element *_element = new Element((LabelContentContainer)TC.tokens[0]);
        return _element;
    }

    if (getType(TC.tokens[0]) == Directive) {
        DirectiveType _dir;
        if (TC.tokens[0] == ".org") _dir = Org;
        else if (TC.tokens[0] == ".align") _dir = Align;
        else if (TC.tokens[0] == ".wfill") _dir = Wfill;
        else if (TC.tokens[0] == ".word") _dir = Word;
        else if (TC.tokens[0] == ".set") _dir = Set;
        else
        	callError(Invalid_Direcive_Error);
            //return NULL;
        Element *_element = new Element(_dir, contentParser(TC.tokens[1], TC.tokens[2]));
        
        return _element;
    }
    OpCodeType _op;
    if (TC.tokens[0] == LD_MQ) _op = Load_MQ;
    else if (TC.tokens[0] == LD_MQ_M) _op = Load_MQ_MX;
    else if (TC.tokens[0] == ST_R) _op = Stor_MX;
    else if (TC.tokens[0] == LD_M) _op = Load_MX;
    else if (TC.tokens[0] == LD_M_NEG) _op = Load_MX_neg;
    else if (TC.tokens[0] == LD_M_ABS) _op = Load_MX_abs;
    else if (TC.tokens[0] == JMP_M) _op = Jump_M;
    else if (TC.tokens[0] == JMP_M_P) _op = Jump_M_P;
    else if (TC.tokens[0] == ADD_M) _op = Add_MX;
    else if (TC.tokens[0] == ADD_M_ABS) _op = Add_MX_abs;
    else if (TC.tokens[0] == SUB_M) _op = Sub_MX;
    else if (TC.tokens[0] == SUB_M_ABS) _op = Sub_MX_abs;
    else if (TC.tokens[0] == MUL_M) _op = Mul_MX;
    else if (TC.tokens[0] == DIV_M) _op = Div_MX;
    else if (TC.tokens[0] == LSH) _op = Lshift;
    else if (TC.tokens[0] == RSH) _op = Rshift;
    else if (TC.tokens[0] == ST_M) _op = Stor_M;
    else
    	callError(Invalid_Instruction_Error);
        //return NULL;
    stripM(&TC.tokens[1]);
    Element *_element = new Element(_op, (InstructionContentContainer)TC.tokens[1]);

    return _element;
}
