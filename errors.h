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


/* Tipos de erros */
typedef enum {
    Invalid_Label_Error,
    Invalid_Direcive_Error,
    Invalid_Instruction_Error,
    Label_Set_Not_Defined,
    Label_Set_Duplicate
    
}ErrorType;

class Error {
 public:
 	void callError(ErrorType _error, int in_line);
 private:
 	ErrorType _err;
 	int* in_line;
};

