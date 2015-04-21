//
// tesk_gkk.cpp
//
#include <mach/mach_time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

#include "types.h"
#include "reading.h"
#include "assembler.h"
#include "errors.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Input file path needed." <<  endl;
        return 1;
    }

    string outputFile = "a.out";

    if (argc == 4 && string(argv[2]).compare("-o") == 0) {
        outputFile = string(argv[3]);
    }

    fstream inputFS;
    string tempStr;

    inputFS.open(argv[1], fstream::in);

    if(!inputFS.is_open()) {
        cout << "Could not open file." << endl;
    }

    MemoryMap* memMap = new MemoryMap();

    while (getline(inputFS, tempStr, '\n')) {
        // Strip comments out of input line
        tempStr = getLineNoCommentLowercase(tempStr);
        if (tempStr.length() != 0) {
            // Cria Element somente dos tokens correspondentes ao label, se existir
            memMap->add(getElement(tokenize(splitLabel(&tempStr))));
            // Cria Element de diretiva ou instrução
            memMap->add(getElement(tokenize(tempStr)));
        }
    }

    inputFS.close();

    memMap->finishUp();

    fstream outputFS;
    outputFS.open(outputFile.c_str(), fstream::out);

    memMap->printMemoryMap(&outputFS);

    outputFS.close();

    return 0;
}


