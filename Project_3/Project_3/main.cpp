#include <iostream>
#include <fstream>

#include "LZ.h"
using namespace std;

int main()
{
	string line;
	string wholeString = "";
	string fileName = "words.txt";
	ifstream myfile(fileName.c_str());

	if (myfile.is_open())
	{
		cout << "File Opened" << endl;
		while (getline(myfile, line))
		{
			wholeString.append(line);
		}
		myfile.close();
	}
	else{
		cout << "Unable to open file" << endl;
	}

	//Creating the LZ class and passing in the string to be compressed
	//Implement Parameters
	LZ encoder(wholeString);
	encoder.startEncode();

	//encoder.generatePermutations("abd");
	encoder.printMap();

	return 0;
}