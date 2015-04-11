
#include <iostream>
#include <fstream>
#include <sstream>
#include <mach/mach_time.h>

uint64_t        start_, end_, elapsed_;

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
	    printf("%s", "File path needed\n");
	    return 1;
	}

	stringstream data;

	//start_ = mach_absolute_time();
	//end_ = mach_absolute_time();
	//elapsed_ = end_ - start_;

	std::fstream fs;
	fs.open(argv[1], std::fstream::in);
	data << fs.rdbuf();

	//getline(cin, dataString);


	cout << elapsed_ << "µs" << endl;

	//while(data.peek() != '\r' && data.peek() != EOF && data.peek() != '\n')
	//{
	//	cout << data;
	//}

	cout << "Output: " << fs.rdbuf() << endl;


}