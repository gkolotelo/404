
#include <iostream>
#include <fstream>
#include <sstream>
#include <mach/mach_time.h>
#include "types.cpp"
#include "reading.cpp"

uint64_t        start_, end_, elapsed_;

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
	    printf("%s", "File path needed\n");
	    return 1;
	}

	fstream fs;
	string tempStr, tempStr2;
	//int pos;
	//TokenContainer TC;
	//char tempStrCA[100];
	//char *token1, *token2;

	start_ = mach_absolute_time();

	fs.open(argv[1], fstream::in);

	while(getline(fs, tempStr, '\n'))
	{
		tempStr = getLineNoCommentLowercase(tempStr); // Strip comments out of input line
		getElement(tokenize(splitLabel(&tempStr))); // Cria Element somente dos tokens correspondentes ao label, se existir
		getElement(tokenize(tempStr)); // Cria Element de diretiva ou instrução
		//cout << tempStr << endl;
		//if (tempStr.length() != 0)
		//{
		//	pos = tempStr.find(':');
		//	//cout << tempStr << endl;
		//	if(pos != -1) // Linha possui label:
		//	{
		//		getElement(tokenize(tempStr.substr(0, pos + 1))); // Cria Element somente do label, se existir
		//		tempStr = tempStr.substr(pos + 1);
		//	}
		//	getElement(tokenize(tempStr)); // Cria Element de diretiva ou instrução
		//}
		
		
	}

	end_ = mach_absolute_time();
	elapsed_ = end_ - start_;
	cout << elapsed_ << "µs" << endl;


}


