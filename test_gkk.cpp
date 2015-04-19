
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
	TokenContainer TC;
	int pos;
	char tempStrCA[100];
	char *token1, *token2;

	start_ = mach_absolute_time();

	fs.open(argv[1], fstream::in);

	while(getline(fs, tempStr, '\n'))
	{
		tempStr = getLineNoCommentLowercase(tempStr); // Strip comments out of input line
		//cout << tempStr << endl;
		if (tempStr.length() != 0)
		{
			pos = tempStr.find(':');
			//cout << "POS: " << pos << endl;
			if(pos != -1) // Has label
			{
				getElement(tokenize(tempStr.substr(0, tempStr.find(':') + 1)));
				// getElements();
				getElement(tokenize(tempStr.substr(pos + 1)));
				// getElements();
			}
			else
			{
				getElement(tokenize(tempStr));
				// getElements();
			}

			//stringstream data;
			//strcpy(tempStrCA, tempStr.c_str());
			//tokenize(tempStrCA, &TC);
			//cout << TC.amount << '|';
			//for(int i=0; i < 5; i++)
			//{
			//	cout << getType(TC.tokens[i]) << '|';
			//}
			//cout << endl;

			
		}
		
	}

	end_ = mach_absolute_time();
	elapsed_ = end_ - start_;
	cout << elapsed_ << "µs" << endl;


}


