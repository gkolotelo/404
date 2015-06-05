#include "errors.h"


/* Funcao para encerrar o processo e informar o tipo de erro. */
void callError(ErrorType _error) {
	std::stringstream errorMessage;
	
	errorMessage << "Error : ";

	switch(_error){
		case Invalid_Label_Error:
			errorMessage << "Invalid label name. Must not start with numbers (0-9)";
			break;
		case Invalid_Direcive_Error:
			errorMessage << "Invalid directive found.";
			break;
		case Invalid_Instruction_Error:
			errorMessage << "Invalid instruction found.";
			break;
		case Label_Set_Not_Defined:
			errorMessage << "Label or set value not defined.";
			break;
		case Label_Set_Duplicate:
			errorMessage << "Label or set value redefinition.";
			break;
        case Max_Memory_Lines:
            errorMessage << "Memory full (filled 1024 lines od data).";
            break;
        case Must_Have_M_Identifier:
            errorMessage << "Instruction not formated correctly. Must be as such 'INST M(X)'.";
            break;
        case NaN:
            errorMessage << "Invalid number. Must be Hex, Oct, Dec or binary.";
            break;
	}
	printf("%s\n", errorMessage.str().c_str());
	exit(EXIT_FAILURE);
}