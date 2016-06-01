#include "LZ.h"


//input constants
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

//credit for conversion function(with a minor tweak) http://stackoverflow.com/a/2687562
std::vector< bool > get_bits(unsigned long x, int numBits) {
	std::string chars(std::bitset< sizeof(long) * CHAR_BIT >(x)
		.to_string(char(0), char(1)));
	chars = chars.substr(chars.length()-numBits, chars.length());
	return std::vector< bool >(chars.begin(), chars.end());
}

LZ::LZ(string filename){
	ifile.open(filename, ios::binary | ios::in);
	cout << ifile.is_open() << endl;
	//cout << "File Name: " << fileName << endl;
}

void LZ::myEncode(char n , char l , char s )
{
	cout << "encoding..."<<endl;
	N = n;
	L = l;
	S = s;
	//default values: N = 11, L = 4, S = 3
	if (ifile.is_open()) {
		cout << "file is opened..." << endl;
		W = pow(2, N);
		F = pow(2, L)-1;
		char* buffer = new char[F];
		window = "";
		lookahead = "";
		//while there is input to process 
		//(i.e. there is unread data or unprocessed data in the lookahead)
		while (!ifile.eof()|| lookahead!="") {
			//if there is unread data
			if (!ifile.eof())
			{//read enough chars(at most F) to make lookahead contain F chars
				ifile.read(buffer, F - lookahead.length());
				//only append the number of chars that were read in the previous step to the
				//lookahead(otherwise the rest of the data will be junk data) if you dont use
				//gcount, at the end of the file you will try to append more chars than you have
				lookahead.append(buffer, ifile.gcount());
			}
			//make sure the size of window is correct
			if (window.length() + lookahead.length() > W) {
				//resize window so that len(window) + len(lookahead)<=W 
				int window_start= window.length() - (W - lookahead.length());
				int window_end	= W - lookahead.length();
				window = window.substr(window_start,window_end);
			}
			findMatch();
		}
		ifile.close();
	}
	//now we need to encode the completed tokens to a file and output the reuslt
	//cleanTokens();
	writeTokens();

}

void LZ::findMatch()
{
	int offset = 0;
	string s = "";
	triple temp = triple();
	//expand through the lookahead, looking for matching substrings of size
	//2 or larger in the window
	for (int len = 2; len < lookahead.size(); ++len)
	{	// grab temporary substring s
		s = lookahead.substr(0, len);
		int index = window.find(s);
		//if we find a match of len 2 or more
		if (index != string::npos)
		{
			temp.s = s;
			temp.offset = window.length() - index;
			temp.length = s.length();
		}
		else
		//either there is no match of 2 or more or we have iterated past the 
		//bounds of the match, in either case, we are done expanding
		{
			/*
			//here index == string::npos, which is equivalent to -1 because
			// it is the largest possible unsigned (int, in this case)
			if (temp.s.length()-1==window.length()) 
			{
				int next = 0;
				while (len + next <= F && temp.s+lookahead[next]==s)
				{
					temp.s += lookahead[next];
					s = temp.s + lookahead[len + next];
					++next;
				}
			}*/
			break;
		}
	}
	//now that we have finished expanding
	//if we did not find a match, just output the character at the beginning of 
	//our lookahead
	if (temp.s =="")
	{	temp.s = lookahead[0];
		temp.length = 0;
		temp.offset = 1;
	}
	if (temp.s.length() < F)
	{	lookahead = lookahead.substr(temp.s.length(), lookahead.length());
	}
	else//temp.s.length==F->empty lookahead
	{	lookahead = "";
	}
	//now append the best match(temp.s) to window(we will resize it when 
	//we go through the loop again)
	window += temp.s;
	//append temp to tokens
	tokens.push_back(temp);
}
void LZ::cleanTokens()
{
	cout << "cleaning Tokens"<<endl;
	deque<triple> temp = tokens;
	tokens = deque<triple>();
	triple current;
	while (temp.size() > 1)
	{
		current = temp.front();
		cout << "<" << current.length << "," << current.offset
			<< "," << current.s << ">" << endl;
		temp.pop_front();
		if (current.length == 0 && temp.front().length == 0)
		{
			temp.front().offset += current.offset;
			temp.front().s = current.s + temp.front().s;
		}
		else
		{
			tokens.push_back(current);
		}
	}
	tokens.push_back(temp.front());
}
void LZ::writeTokens()
{
	deque<triple> temp = tokens;
	cout << N << "," << L << "," << S << endl;
	vector<bool>Nb = get_bits(N,8);
	vector<bool>Lb = get_bits(L,8);
	vector<bool>Sb = get_bits(S,8);
	//ouput NLS as bytes
	vector<bool>output;
	output.reserve(24);
	output.insert(output.end(), Nb.begin(), Nb.end());
	output.insert(output.end(), Lb.begin(), Lb.end());
	output.insert(output.end(), Sb.begin(), Sb.end());

	std::ostream_iterator<bool>out_it(std::cerr);
	copy(output.begin(), output.end(), out_it);
	while (temp.size() > 0)
	{
		writeToken(temp.front());
		temp.pop_front();
	}
	triple endToken;
	endToken.length = 0;
	endToken.offset = 0;
	endToken.s = "";
	writeToken(endToken);
}
void LZ::writeToken(triple token) {
	//if it is a literal token, write out the
	//code, strlen, and literal string
	vector<bool>len;//L
	vector<bool>offset;//N
	vector<bool>strlen;//S
	vector<bool>chars(0);
	vector<bool>output;
	std::ostream_iterator<bool>out_it(std::cerr);
	if (token.length==0)
	{
		if (token.offset == 0)
		{
			len = get_bits(token.length, L);
			offset = get_bits(token.offset, S);
			output.insert(output.end(), len.begin(), len.end());
			output.insert(output.end(), offset.begin(), offset.end());
			goto endif;//I am so sorry for this goto
		}
		//write out L 0's
		len = get_bits(token.length,L);
		//write out offset(the number of chars) in S bits
		offset = get_bits(token.offset, S);
		//write out the chars as bytes
		for (int i = 0; i < token.s.length(); ++i)
		{
			vector<bool>character = get_bits(token.s[i], 8);
			chars.insert(chars.end(), character.begin(), character.end());
		}
		output.insert(output.end(), len.begin(), len.end());
		output.insert(output.end(), offset.begin(), offset.end());
		output.insert(output.end(), chars.begin(), chars.end());
		
	}
	//if it is a reference, write out
	//matchlen,offset
	else
	{
		//write out length in L bits
		len = get_bits(token.length, L);
		//write out offset in N bits
		offset = get_bits(token.offset, N);

		output.insert(output.end(), len.begin(), len.end());
		output.insert(output.end(), offset.begin(), offset.end());
	}
	endif:
	copy(output.begin(), output.end(), out_it);
}
//void LZ::startStEncode(){
//	window = "";
//	cout << "Start Encoding!" << endl;
//	for (int i = 0; i < wholetext.length(); i++){
//		
//		int lenMatch = 1;
//		string slidingChars = wholetext.substr(i,lenMatch);
//		if (window.find(slidingChars) != string::npos){ //If 1 char match is found
//			//cout << "Match Found" << endl;
//			for (int j = i+1; j < wholetext.length(); j++){	
//				lenMatch++;
//				slidingChars = wholetext.substr(i, lenMatch);
//				if ((window+wholetext.substr(i,slidingChars.length()-1)).find(slidingChars) != string::npos){
//					continue;
//				}
//				else{
//					slidingChars = wholetext.substr(i, lenMatch - 1);
//
//					break;
//				}
//			}
//		}
//		if (slidingChars.length() > 1){
//			//cout << "Combo:	"<<slidingChars << endl;
//			i += (slidingChars.length()-1);
//		}
//		//else{
//		//	cout << slidingChars << endl;
//		//}
//		
//		
//		window = wholetext.substr(0,i+1);
//	}
//}
//
//void LZ::startEncode(int N, int L , int S){
//	//Starts looping over the whole text
//	// Initialize slide Beginning position and start sliding
//	cout << "Printing:  " << endl;
//	int lenMatch = 1;
//	for (int i = 0; i < wholetext.length(); i++){
//		int startIndex = i;
//		string slidingChars = wholetext.substr(startIndex,lenMatch);
//		//cout << "Here1: "<<slidingChars << endl;
//		bool match = false;
//		while (dict.count(slidingChars) == 1){			//If match is found
//			if (lenMatch + startIndex < wholetext.length()){
//				//cout << "Here" << endl;
//				lenMatch++;									//Increase counter to go one more
//			}
//			else{
//				break;
//			}
//
//			slidingChars = wholetext.substr(startIndex, lenMatch);
//			//cout << "Sliding CHars:  " << slidingChars << endl;
//			match = true;
//			//cout << "match" << endl;
//		}
//		if (match){
//			slidingChars = wholetext.substr(startIndex, lenMatch - 1);
//			match = false;
//		}
//		
//		if (slidingChars.length() > 1){ //If more than 1 match is found
//			cout << "("<<slidingChars.length()<<", "<<window.length()-dict[slidingChars]<<") "<<endl;
//			i += (slidingChars.length());
//		}
//		else{
//			cout << slidingChars << endl;
//		}
//		
//		cout << "i: " << i<< endl;
//		//cout << "Printing: "<<slidingChars;
//		//cout << "Here 3:  " << slidingChars << endl;
//		lenMatch = 1;
//		generatePermutations(slidingChars);
//	}
//}
//
//void LZ::generatePermutations(string updatedString){
//	//To DO:
//	//Cut down new_window accordingly
//	//check if we are at the beginning then do accordingly
//	//cout << updatedString<< endl;
//	string new_window = window + updatedString;			//Creates a new long window, currentwindow+buffer
//	//cout << "New Window: " << new_window<<endl;
//	int len = updatedString.length();
//	//int num_loop = len*window.length()+len;
//	int num_loop = calc_num_perms(new_window.length(),window.length());
//	int totalLen = new_window.length();
//	//cout << "New Window: "<<new_window << endl;
//	int start = 0;
//	
//	for (int i = 0; i <num_loop; i++){
//		string inserted_string = new_window.substr(start, totalLen-start);
//		//dict.insert(pair<string,int>(inserted_string,start));
//		dict[inserted_string] = start;
//		start++;
//		if ((totalLen - start) == 0){
//			start = 0;
//			totalLen--;
//		}
//
//	}
//	window = new_window;
//
//}
///*
//void LZ::printMap(){
//	//Printing the map
//	cout << "\nPrinting the Map" << endl;
//	for (std::map< string,int >::const_iterator iter = dict.begin();
//		iter != dict.end(); ++iter)
//		cout << iter->first << '\t' << iter->second << '\n';
//}
//*/
//
//int LZ::calc_num_perms(int n,int m){
//	int total = n*(n + 1) / 2;
//	int diff = m*(m + 1) / 2;
//	return total - diff;
//}