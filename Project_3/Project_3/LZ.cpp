#include "LZ.h"
//#include <string>
  
//imput constants
const int NMAX = 14;
const int NMIN = 9;
const int LMAX = 4;
const int LMIN = 3;
const int SMAX = 5;
const int SMIN = 1;

//utility function
bool isbetween(int x, int min, int max)
{
	return(min <= x && x <= max);
}

//constructor
LZ::LZ(string filename){
	setWholetext(filename);
	cout << "Whole Text: " << wholetext << endl;
}


void LZ::startEncode(int N, int L, int S ){
	cout << "Encoding:  N:" << N << "  L:" << L << "  S:" << S << endl;
	//assert that the input values are within the specified bounds
	assert(isbetween(N, NMIN, NMAX));
	assert(isbetween(L, LMIN, LMAX));
	assert(isbetween(S, SMIN, SMAX));
	int windowSize		= pow(2, N);
	int lookaheadSize	= pow(2, L) - 1;
	int longestLiteral	= pow(2, S) - 1;
	cout << "WS: " << windowSize << "  LS:" << lookaheadSize << "  LL:" << longestLiteral << endl;
	//Starts looping over the whole text
	// Initialize slide Beginning position and start sliding
	
	int lenMatch = 1;
	for (int i = 0; i < wholetext.length(); i++){
		int startIndex = i;
		//cout << "SI:" << startIndex << endl;
		string slidingChars = wholetext.substr(startIndex,lenMatch);
		//cout << "Here1: "<<slidingChars << endl;
		bool match = false;
		while (dict.count(slidingChars) == 1){			//If match is found
			lenMatch++;									//Increase counter to go one more
			slidingChars = wholetext.substr(startIndex, lenMatch);
			cout << "Sliding Chars:  " << slidingChars << endl;
			match = true;
		}
		if (match){
			slidingChars = wholetext.substr(startIndex, lenMatch - 1);
		}
		if (slidingChars.length() > 1){ //If more than 1 match is found
			cout << "("<<slidingChars.length()<<", "<<window.length()-dict[slidingChars]<<") "<<endl;
			i += slidingChars.length()-1;
		}
		else{
			cout <<"i: "<<i<<" SC: "<< slidingChars << endl;
		}

		//cout << "Printing: "<<slidingChars;
		//cout << "Here 3:  " << slidingChars << endl;
		lenMatch = 1;
		generatePermutations(slidingChars);
	}
}

void LZ::generatePermutations(string updatedString){
	//To DO:
	//Cut down new_window accordingly
	//check if we are at the beginning then do accordingly
	//cout << updatedString<< endl;
	string new_window = window + updatedString;			//Creates a new long window, currentwindow+buffer
	//cout << "New Window: " << new_window<<endl;
	int len = updatedString.length();
	//int num_loop = len*window.length()+len;
	int num_loop = calc_num_perms(new_window.length(),window.length());
	int totalLen = new_window.length();
	//cout << "New Window: "<<new_window << endl;
	int start = 0;
	
	for (int i = 0; i <num_loop; i++){
		string inserted_string = new_window.substr(start, totalLen-start);
		//dict.insert(pair<string,int>(inserted_string,start));
		dict[inserted_string] = start;
		start++;
		if ((totalLen - start) == 0){
			start = 0;
			totalLen--;
		}
	}
	window = new_window;
}

void LZ::printMap(){
	//Printing the map
	cout << "\nPrinting the Map" << endl;
	for (std::map< string,int >::const_iterator iter = dict.begin();
		iter != dict.end(); ++iter)
		cout << iter->first << '\t' << iter->second << '\n';
}


int LZ::calc_num_perms(int n,int m){
	int total = n*(n + 1) / 2;
	int diff = m*(m + 1) / 2;
	return total - diff;
}