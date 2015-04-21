#include "errors.h"


/* Funcao para encerrar o processo e informar o tipo de erro. */
void callError(ErrorType _error) {
	std::stringstream errorMessage;
	
	errorMessage << "Error : "

	switch(_error){
		case Invalid_Label_Error:
			errorMessage << "";
			break;
		case Invalid_Direcive_Error:
			errorMessage << "";
			break;
		case Invalid_Instruction_Error:
			errorMessage << "";
			break;
		case Label_Set_Not_Defined:
			errorMessage << "Label or set name not defined.";
			break;
		case Label_Set_Duplicate:
			errorMessage << "Label or set name duplicate.";
			break;
	}
	printf("%s\n", errorMessage.str()o);
	exit(EXIT_FAILURE);
}