// reading.cpp
// Methods for input reading and processing


using namespace std;

#ifndef types_cpp
	#include "types.cpp"
#endif
#define reading_cpp


// Returns string up to comment (delimited by '#')
string getLineNoCommentLowercase(string in)
{
	// try using strtok to avoid converting to stream!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	string tempStr;
	stringstream data;
	data << in;
	getline(data, tempStr, '#');
	transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::tolower);
	return tempStr;
}

ElementType getType(char *in)
{
	if(strchr(in, ':') != NULL)
		return Label;
	if(strchr(in, '.') != NULL)
		return Directive;
	return Instruction;
}

void tokenize(char *in, TokenContainer *TC)
{
	int i = 0;
	stringstream data;
	char *token;

	token = strtok(in, ", \t");

	while(token != NULL)
	{
		(TC->tokens[i++]).assign(token);
		//cout << token << '|';
		token = strtok(NULL, ", \t");
	}
	TC->amount = i; // Store number of tokens on zero index
	//cout << TC->amount << '|';
	//for(i=0; i < 5; i++)
	//{
	//	cout << TC->tokens[i] << '|';
	//}
	//cout << endl;
}

Element getElement(TokenContainer TC)
{
	int i = 0;

}


//string *tokenize(char *in)
//{
//	string tokens[5];
//	int i = 0;
//	stringstream data;
//	char *label, *directive, *instruction, *aux;
//
//	label = strtok(in, ":");
//	remaining = strtok(NULL, ".");
//	if(remaining == NULL) //INVALID!! Label is only element of line
//	{
//
//
//	}
//	dir_or_int = strtok(remaining, ".");
//	remaining = strtok(NULL, ".");
//	if(remaining == NULL) // Instruction is only remaining element of line
//	{
//		// Parse instruction:
//
//	}
//	// Parse directive:
//	cout << (token == NULL) << endl;
//
//	while(token != NULL)
//	{
//		data << token << '|';
//
//		//if(getType(token) == label)
//		//{
////
//		//}
//		//else if
//
//		token = strtok(NULL, "5");
//	}
//	cout << data.rdbuf() << endl;
//}

//string *tokenize_old(char *in)
//{
//	string tokens[5];
//	int i = 0;
//	stringstream data;
//	char *token;
//
//	//cout << "|" << strtok(in, "(") << "|" << endl;
//
//	token = strtok(in, ", \t");
//
//	while(token != NULL)
//	{
//		cout << token << '|';
//		token = strtok(NULL, ", \t");
//	}
//	cout << token << endl;
//}