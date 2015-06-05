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
    Label_Set_Duplicate,
    Max_Memory_Lines,
    Must_Have_M_Identifier,
    NaN
    
}ErrorType;

/* Funcao para encerrar o processo e informar o tipo de erro. */
void callError(ErrorType _error);
