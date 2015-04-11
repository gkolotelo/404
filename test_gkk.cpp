
#include <iostream>
#include <fstream>
#include <sstream>
#include <mach/mach_time.h>

uint64_t        start_, end_, elapsed;
char data[10001];

using namespace std;

int main(int argc, char *argv[])
{
	//if(argc != 2)
    //{
    //    printf("%s", "File path needed\n");
    //    return 1;
    //}

    start_ = mach_absolute_time();

    //std::fstream fs;
    //fs.open(argv[1], std::fstream::in);

    cin >> data;

	end_ = mach_absolute_time();
	elapsed = end_ - start_;

	//fs.read(data, 10000);

	cout << elapsed << "µs" << endl;




	cout << "Output: " << data << endl;


}