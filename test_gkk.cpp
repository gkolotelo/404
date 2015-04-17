
#include <iostream>
#include <fstream>
#include <sstream>
#include <mach/mach_time.h>

uint64_t        start_, end_, elapsed_;

using namespace std;

// Returns string up to comment (delimited by '#')
string getLineNoComment(string in)
{
	string tempStr;
	stringstream data;
	data << in;
	getline(data, tempStr, '#');
	return tempStr;
}

list<string> tokenize(string in)
{

}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
	    printf("%s", "File path needed\n");
	    return 1;
	}

	fstream fs;
	string tempStr;
	char tempStrCA[100];
	char *token;

	start_ = mach_absolute_time();

	fs.open(argv[1], fstream::in);

	while(getline(fs, tempStr, '\n'))
	{
		tempStr = getLineNoComment(tempStr); // Strip comments out of input line
		if (tempStr.length() != 0)
		{
			stringstream data;
			//cout << tempStr << endl;
			strcpy(tempStrCA, tempStr.c_str());
			token = strtok(tempStrCA, " \t");
			data << '|' << token << '|';
			while(token != NULL)
			{
				token = strtok(NULL, " \t");
				if(token != NULL) data << token << '|';
			}
			cout << data.rdbuf() << endl;
		}
		
		
	}

	end_ = mach_absolute_time();
	elapsed_ = end_ - start_;
	cout << elapsed_ << "µs" << endl;


}


