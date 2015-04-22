/*
 *  MC404 - Organizacao de Computadores e Linguagem de Montagem 
 *
 *  Trabalho prático 1 - O trabalho consiste em implementar em 
 *  C/C++ um metodo de traducao de uma linguagem de montagem 
 *  simples para uma representacao intermediaria adequada para 
 *  simulacao em uma arquitetura moderna.
 *  
 *  Turma E / 1s2015
 *
 *  Alunos: 135964 Guilherme Kairalla Kolotelo
 *          137943 Alexandre Seidy Ioshisaqui
 */

#include <mach/mach_time.h>


#include <fstream>
#include <string>


#include "errors.h"
#include "types.h"
#include "reading.h"
#include "assembler.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (string(argv[1]).compare("--help") == 0) {
        cout << "ra135964_ra137943 IAS Assembler, Authors: " << endl;
        cout << "   Alexandre Seidy Ioshisaqui and Guilherme Kairalla Kolotelo" << endl << endl;
        cout << "Usage: " << endl;
        cout << "   ra135964_ra137943 [options] <input_text_file>" << endl << endl;
        cout << "Options:" << endl;
        cout << "   --help              Help file." << endl;
        cout << endl;
        return 0;
    }
    else if (argc != 2) {
        cout << "Input file path not specified. Try '--help' for more information." << endl;
        return 1;
    }

    fstream inputFS;
    string tempStr;

    inputFS.open(argv[1], fstream::in);

    if(!inputFS.is_open()) {
        cout << "Could not open file. Try '--help' for more information." << endl;
    }

    MemoryMap* memMap = new MemoryMap();

    while (getline(inputFS, tempStr, '\n')) {
        // Remove comentarios da linha de entrada, e converte para minúsculo
        tempStr = getLineNoCommentLowercase(tempStr);
        if (tempStr.length() != 0) {
            // Cria Element somente dos tokens correspondentes ao label, se existir
            memMap->add(getElement(tokenize(splitLabel(&tempStr))));
            // Cria Element de diretiva ou instrução
            memMap->add(getElement(tokenize(tempStr)));
        }
    }

    inputFS.close();

    // Preparação para imprimir
    memMap->finishUp();

    stringstream outputSS;

    // Imprime em stream
    memMap->printMemoryMap(&outputSS);

    // Envia para stdout
    cout << outputSS.rdbuf();

    return 0;
}


