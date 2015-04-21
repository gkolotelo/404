// reading.cpp
// Methods for input reading and processing
#include "reading.h"
using namespace std;


// Returns string up to comment (delimited by '#')
string getLineNoCommentLowercase(string in) {
    // try using strtok to avoid converting to stream!!!!!!!!!!!!!!!
    string tempStr;
    stringstream data;
    data << in;
    getline(data, tempStr, '#');
    transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
    return tempStr;
}

ElementType getType(string in) {
    if (in.back() == ':')  // Would change order of directive/label increase efficiency?
        return Label;
    if (in.front() == '.')
        return Directive;
    return Instruction;  // Or content!!!!!! Maybe say it's invalid type, depending on implementation
}

string splitLabel(string *in) {
    // cout << "splitLabel in: " << *in << endl;
    int pos = in->find(':');
    if (pos == -1) return "";
    string tmp = in->substr(0, pos + 1);
    *in = in->substr(pos + 1);
    // cout << "splitLabel new in: " << *in << endl;
    // cout << "splitLabel retval: " << tmp << endl;
    return tmp;
}

void stripM(string *in) {
    if (in->length() == 0) return;
    if (in->front() != 'm')
        ;  // ADD_ERROR must have M() identifier
    in->pop_back();
    in->erase(0,2);
}

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
        // cout << token << '|';
        token = strtok(NULL, ", \t");
    }
    TC.amount = i;  // Store number of tokens on zero index
    // cout << TC->amount << '|';
    // for(i=0; i < 5; i++)
    // {
    //   cout << TC->tokens[i] << '|';
    // }
    // cout << endl;
    return TC;
}


Element* getElement(TokenContainer TC) {
    if (TC.amount == 0) return NULL;  // No tokens to get Element from

    if (getType(TC.tokens[0]) == Label) {
        TC.tokens[0].pop_back();
        Element *_element = new Element((LabelContentContainer)TC.tokens[0]);
        //cout << "Label: " << _element->GetLabelContentContainer() << endl;
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
            return NULL;
        // ADD_ERROR se nenhuma instrucao foi encontrada retornar erro de instrucao invalida
        Element *_element = new Element(_dir, contentParser(TC.tokens[1], TC.tokens[2]));
        //cout << "Directive: " << _element->GetDirectiveType() << " Content 1: " << _element->GetDirectiveContentContainer().content1 << " Content 2: " << _element->GetDirectiveContentContainer().content2 << endl;
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
        return NULL;
    // ADD_ERROR se nenhuma instrucao foi encontrada retornar erro de instrucao invalida
    stripM(&TC.tokens[1]);
    Element *_element = new Element(_op, (InstructionContentContainer)TC.tokens[1]);
    //cout << "Instruction: " << _element->GetOpCodeType() << " Content: " << _element->GetInstructionContentContainer() << endl;

    return _element;
}


//string *tokenize(char *in)
//{
//  string tokens[5];
//  int i = 0;
//  stringstream data;
//  char *label, *directive, *instruction, *aux;
//
//  label = strtok(in, ":");
//  remaining = strtok(NULL, ".");
//  if(remaining == NULL) //INVALID!! Label is only element of line
//  {
//
//
//  }
//  dir_or_int = strtok(remaining, ".");
//  remaining = strtok(NULL, ".");
//  if(remaining == NULL) // Instruction is only remaining element of line
//  {
//      // Parse instruction:
//
//  }
//  // Parse directive:
//  cout << (token == NULL) << endl;
//
//  while(token != NULL)
//  {
//      data << token << '|';
//
//      //if(getType(token) == label)
//      //{
////
//      //}
//      //else if
//
//      token = strtok(NULL, "5");
//  }
//  cout << data.rdbuf() << endl;
//}

//void tokenize_old(char *in, TokenContainer *TC)
//{
//  int i = 0;
//  stringstream data;
//  char *token;
//
//  token = strtok(in, ", \t");
//
//  while(token != NULL)
//  {
//      (TC->tokens[i++]).assign(token);
//      //cout << token << '|';
//      token = strtok(NULL, ", \t");
//  }
//  TC->amount = i; // Store number of tokens on zero index
//  //cout << TC->amount << '|';
//  //for(i=0; i < 5; i++)
//  //{
//  //  cout << TC->tokens[i] << '|';
//  //}
//  //cout << endl;
//}

//string *tokenize_old(char *in)
//{
//  string tokens[5];
//  int i = 0;
//  stringstream data;
//  char *token;
//
//  //cout << "|" << strtok(in, "(") << "|" << endl;
//
//  token = strtok(in, ", \t");
//
//  while(token != NULL)
//  {
//      cout << token << '|';
//      token = strtok(NULL, ", \t");
//  }
//  cout << token << endl;
//}

// DirectiveContentContainer contentParser(string in) {
//     DirectiveContentContainer CC;
//     char *token;
//     token = strtok((char *)in.c_str(), ", \t");
//     CC.content1 = convertValue(token);
//     token = strtok(NULL, ", \t");
//     if (token == NULL) {
//         CC.amount = 1;
//         return CC;
//     }
//     CC.content2 = convertValue(token);
//     CC.amount = 2;
//     return CC;
// }

// DirectiveContentContainer contentParser(string token1, string token2) {
//     DirectiveContentContainer CC;
//     CC.content1 = convertValue(token1);
//     if (token2 == "") {
//         CC.amount = 1;
//         return CC;
//     }
//     CC.content2 = convertValue(token2);
//     CC.amount = 2;
//     return CC;
// }