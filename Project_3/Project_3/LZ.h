#ifndef LZ_H
#define LZ_H
#include <string>
#include <iostream>
#include <cassert>
#include <map>
#include <math.h>
//"using namespace std;" is dangerous, just use what is required for readability
using std::cout;
using std::endl;
using std::string;
using std::map;

class LZ{
	private:
		string wholetext;
		string window=""; //Stores string that has already been processed
		string lookahead; //Stores the lookahead buffer
		map<string, int> dict; //String=actual string or char, int =index of that string
	public:
		LZ(string filename);
		void setWholetext(string text){ wholetext = text; };
		void startEncode(int N = 11, int L = 4, int S = 3);
		void generatePermutations(string);	//Inserts into the map all the possible combos of matchings, including the new string that was matched
		void printMap();
		int calc_num_perms(int,int); //Calculate how many times the for loop in genereatePerm needs to iterate
};



#endif

/*
Now:
	Read string file
	Start the slider
		Read letter:
			Does it match any patterns?
			Yes:
				Check next letter
			No:
				Insert into window dictionary
				make permutations
*/