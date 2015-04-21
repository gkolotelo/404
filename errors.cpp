#include "errors.h"

void Error::callError(ErrorType _error, int in_line) {
	std::stringstream errorMessage;
	
	errorMessage << "Error : "
	if(in_line != -1)
		errorMessage << "Line " << in_line << ": ";

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
	printf("%s\n", errorMessage.str()o); //printf ou fprintf ?
	exit(EXIT_FAILURE);
}