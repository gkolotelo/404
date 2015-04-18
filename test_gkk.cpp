
#include <iostream>
#include <fstream>
#include <sstream>
#include <mach/mach_time.h>
#include "types.cpp"
#include "reading.cpp"


uint64_t        start_, end_, elapsed_;

using namespace std;







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

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
	    printf("%s", "File path needed\n");
	    return 1;
	}

	fstream fs;
	string tempStr;
	string tokens[5];
	char tempStrCA[100];

	start_ = mach_absolute_time();

	fs.open(argv[1], fstream::in);

	while(getline(fs, tempStr, '\n'))
	{
		tempStr = getLineNoCommentLowercase(tempStr); // Strip comments out of input line
		if (tempStr.length() != 0)
		{
			//cout << tempStr << endl;
			stringstream data;
			//cout << tempStr << endl;
			strcpy(tempStrCA, tempStr.c_str());
			tokenize(tempStrCA, tokens);
			//token = strtok(tempStrCA, " \t");
			//data << '|' << token << '|';
			//while(token != NULL)
			//{
			//	token = strtok(NULL, " \t");
			//	if(token != NULL) data << token << '|';
			//}
			//cout << data.rdbuf() << endl;
		}
		
		
	}

	end_ = mach_absolute_time();
	elapsed_ = end_ - start_;
	cout << elapsed_ << "µs" << endl;


}


