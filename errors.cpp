//
// errors.cpp
//
//


typedef enum {
    Invalid_Label_Error,
    Invalid_Direcive_Error,
    Invalid_Instruction_Error,
    Label_Set_Not_Defined,
    Label_Set_Duplicate
    //Labels and sets must not start with nubers
    //more than one label per line
}ErrorType;

class Error {
 public:
 	//startErrorCheck(int* line)
    void callError(ErrorType _error, int in_line);
 private:
 	ErrorType _err;
 	int* in_line
};

//Error::startErrorCheck(int* line){
//	in_line = line;
//}


void Error::callError(ErrorType _error, int in_line) {
	char errorMessage[255];
	if(in_line != -1)
		sprintf(errorMessage, "Error: Line %d: ", in_line)
	else
		sprintf(errorMessage, "Error: ")

	switch(_error){
		case Invalid_Label_Error:
			strcat(errorMessage, "");
			break;
		case Invalid_Direcive_Error:
			strcat(errorMessage, "");
			break;
		case Invalid_Instruction_Error:
			strcat(errorMessage, "");
			break;
		case Label_Set_Not_Defined:
			strcat(errorMessage, "Label or set name not defined.");
			break;
		case Label_Set_Duplicate:
			strcat(errorMessage, "Label or set name duplicate.");
			break;
	}
	printf("%s\n", errorMessage);
	exit(EXIT_FAILURE);
}