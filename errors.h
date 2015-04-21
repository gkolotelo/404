/*
 *	MC404 - Organizacao de Computadores e Linguagem de Montagem 
 *
 *	Trabalho pretico 1 - O trabalho consiste em implementar em 
 *	C/C++ um metodo de traducao de uma linguagem de montagem 
 *	simples para uma representacao intermediaria adequada para 
 *	simulacao em uma arquitetura moderna.
 *	
 *	Turma E / 1s2015
 *
 *	Alunos: 135964 Guilherme Kairalla Kolotelo
 *			137943 Alexandre Seidy Ioshisaqui
 */
#define ERRORS_H

#define EXIT_FAILURE 1

#include <string>
#include <iostream>
#include <sstream>

/* Tipos de erros */
typedef enum {
    Invalid_Label_Error,
    Invalid_Direcive_Error,
    Invalid_Instruction_Error,
    Label_Set_Not_Defined,
    Label_Set_Duplicate
    
}ErrorType;

/* Funcao para encerrar o processo e informar o tipo de erro. */
void callError(ErrorType _error);
