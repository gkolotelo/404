

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
 	int* in_line
};