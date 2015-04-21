

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