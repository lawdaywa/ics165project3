#include <iostream>
#include <fstream>

#include "LZ.h"
using namespace std;

int main()
{
	string line;
	
	string fileName = "kennedy.xls";
	

	//Creating the LZ class and passing in the string to be compressed
	//Implement Parameters
	LZ encoder(fileName);
	encoder.myEncode();

	
	

	return 0;
}