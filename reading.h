// reading.cpp
// Methods for input reading and processing


#include <string>
#include <algorithm>
#include <sstream>


using namespace std;

#ifndef TYPES_H
    #include "types.h"
#endif
#define READING_H

// Remove comentarios de uma string e a converte para 'lowercase'
string getLineNoCommentLowercase(string in);

// Retorna o tipo de Element um token é
ElementType getType(string in);

// Retorna somente a porção correspondente ao label de uma string e modifica a string de entrada removendo a porção de label
string splitLabel(string *in);

// Remove os caracteres 'M()' do argumento de uma instrução
void stripM(string *in);

// Converte um valor de entrada (Hex, Oct, Dec ou binario) para int64
int64_t convertValue(string inStr);

// Retorna o DCC dados até 2 tokens de diretiva
DirectiveContentContainer contentParser(string token1, string token2);

// Retorna um TokenContainer dado uma string de entrada
TokenContainer tokenize(string in);

// Retorna um Element dado um TokenContainer
Element* getElement(TokenContainer TC);