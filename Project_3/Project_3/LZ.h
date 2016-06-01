#ifndef LZ_H
#define LZ_H
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <math.h>
#include <deque>
#include <vector>
#include <bitset>
#include <iterator>
//"using namespace std;" is dangerous, just use what is required for readability
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::deque;
using std::ifstream;
using std::ios;
using std::vector;
using std::bitset;
using std::ostream;

struct triple {
	int length;
	int offset;
	string s;
};

class LZ{
	private:
		ifstream ifile;
		int N, L, S, W, F;
		string window=""; //Stores string that has already been processed
		string lookahead=""; //Stores the lookahead buffer
		map<string, int> dict; //String=actual string or char, int =index of that string
		deque<triple> tokens;// deque of tokens that will eventually be converted to a compressed file
		void findMatch();
		void cleanTokens();
		void writeToken(triple token);
	public:
		LZ(string filename);
		void myEncode(char N = 11, char L = 4, char S = 3);
		void writeTokens();
		//void startEncode(int N = 11, int L = 4, int S = 3);
		//void LZ::startStEncode();
		//void generatePermutations(string);	//Inserts into the map all the possible combos of matchings, including the new string that was matched
		//void printMap();
		//int calc_num_perms(int,int); //Calcula`te how many times the for loop in genereatePerm needs to iterate
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