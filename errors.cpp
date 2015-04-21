//
// errors.cpp
//
//


typedef enum {
    Invalid_Label_Error,
    Invalid_Direcive_Error,
    Invalid_Instruction_Error
    //Labels and sets must not start with nubers
    //more than one label per line
}ErrorType;

class Error {
 public:
 	//startErrorCheck(int* line)
    callError(ErrorType _error, int in_line);
 private:
 	ErrorType _err;
 	int* in_line
};

//Error::startErrorCheck(int* line){
//	in_line = line;
//}


Error::callError(ErrorType _error, int in_line) {
	string errorMessage;
	errorMessage = "Error: "
	strcat(errorMessage)
	switch(_error){
		default:
	}


}