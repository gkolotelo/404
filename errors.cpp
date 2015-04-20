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
    error(ErrorType _error, );
 private:
    ErrorType _err;
};

Error::error(ErrorType _error, int in_line) {

}