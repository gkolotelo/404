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

void tokenize(char *in, string *tokens)
{
	int i = 0;
	stringstream data;
	char *token;

	token = strtok(in, ", \t");

	while(token != NULL)
	{
		(tokens[++i]).assign(token);
		//cout << token << '|';
		token = strtok(NULL, ", \t");
	}
	tokens[0] = i; // Store number of tokens on zero index
	//cout << stoi(tokens[0] + 48) << '|';
	//for(i=1; i < 5; i++)
	//{
	//	cout << tokens[i] << '|';
	//}
	//cout << endl;
	//cout << endl;
}