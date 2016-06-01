#include "LZ.h"
#include <string>

LZ::LZ(string filename){
	setWholetext(filename);
	//cout << "Whole Text: " << wholetext << endl;
}


void LZ::startStEncode(){
	
	cout << "Start Encoding!" << endl;
	for (int i = 0; i < wholetext.length(); i++){
		int startIndex = i;
		int lenMatch = 1;
		string slidingChars = wholetext.substr(startIndex,lenMatch);
		if (wholetext.find(slidingChars) == string::npos){
			cout << "Match Found" << endl;
			for (int j = i+1; j < wholetext.length(); j++){	
				lenMatch++;
				slidingChars = wholetext.substr(startIndex, lenMatch);
				if (wholetext.find(slidingChars) == string::npos){
					continue;
				}
				else{
					slidingChars = wholetext.substr(startIndex, lenMatch-1);

					break;
				}
			}
			cout << "Match: "<<slidingChars<< endl;
		}
		else{
			cout <<wholetext[i]<< endl;
		}
		
	}
}

void LZ::startEncode(){
	//Starts looping over the whole text
	// Initialize slide Beginning position and start sliding
	cout << "Printing:  " << endl;
	int lenMatch = 1;
	for (int i = 0; i < wholetext.length(); i++){
		int startIndex = i;
		string slidingChars = wholetext.substr(startIndex,lenMatch);
		//cout << "Here1: "<<slidingChars << endl;
		bool match = false;
		while (dict.count(slidingChars) == 1){			//If match is found
			if (lenMatch + startIndex < wholetext.length()){
				//cout << "Here" << endl;
				lenMatch++;									//Increase counter to go one more
			}
			else{
				break;
			}

			slidingChars = wholetext.substr(startIndex, lenMatch);
			//cout << "Sliding CHars:  " << slidingChars << endl;
			match = true;
			//cout << "match" << endl;
		}
		if (match){
			slidingChars = wholetext.substr(startIndex, lenMatch - 1);
			match = false;
		}
		
		if (slidingChars.length() > 1){ //If more than 1 match is found
			cout << "("<<slidingChars.length()<<", "<<window.length()-dict[slidingChars]<<") "<<endl;
			i += (slidingChars.length());
		}
		else{
			cout << slidingChars << endl;
		}
		
		cout << "i: " << i<< endl;
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
/*
void LZ::printMap(){
	//Printing the map
	cout << "\nPrinting the Map" << endl;
	for (std::map< string,int >::const_iterator iter = dict.begin();
		iter != dict.end(); ++iter)
		cout << iter->first << '\t' << iter->second << '\n';
}
*/

int LZ::calc_num_perms(int n,int m){
	int total = n*(n + 1) / 2;
	int diff = m*(m + 1) / 2;
	return total - diff;
}