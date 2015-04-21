// reading.cpp
// Methods for input reading and processing


#include <string>
#include <algorithm>

using namespace std;

#ifndef TYPES_H
    #include "types.h"
#endif
#define READING_H


string getLineNoCommentLowercase(string in);
ElementType getType(string in);
string splitLabel(string *in);
void stripM(string *in);
int64_t convertValue(string inStr);
DirectiveContentContainer contentParser(string token1, string token2);
TokenContainer tokenize(string in);
Element* getElement(TokenContainer TC);