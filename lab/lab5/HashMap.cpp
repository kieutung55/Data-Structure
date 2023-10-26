#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "ArgumentManager.h"

using namespace std;

//------------- print result -------------------//

void printInfo(map<string, int> orderedMap, ofstream& outfile) {

	bool is_valid = false;

	for (map<string, int>::const_iterator it = orderedMap.begin(); it != orderedMap.end(); ++it)
	{
		if (it->second > 1) {
			is_valid = true;
		}
	}

	if (is_valid) {
		outfile << "True" << endl;
		for (map<string, int>::const_iterator it = orderedMap.begin(); it != orderedMap.end(); ++it)
		{
			outfile << it->first << ": " << it->second << endl;
		}
	}

	else {
		outfile << "False" << endl;
	}
	
}

int main(int argc, char* argv[])
{
	ArgumentManager AM(argc, argv);
	
	string input_filename = AM.get("input");            // file name for input
	string out_filename = AM.get("output");             // file name for output
	ifstream infile;                            // filestream for reading of input file
	ofstream outfile;                           // filestream for writing to output file
	infile.open(input_filename);				// open file for read
	outfile.open(out_filename);					// open file for write

	map<string, int> orderedMap;				// create orderedMap
	string word;
	int email_num = 0;

	if (!infile.fail()) {
		while (!infile.eof()) {

			infile >> word;
		
			word.erase(remove(word.begin(), word.end(), '\n'), word.end());
			word.erase(remove(word.begin(), word.end(), '\r'), word.end());

			if (word == "Dear") {
				if (email_num == 0) {
					infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					email_num++;
					continue;
				}
				else if(email_num > 0) {
					printInfo(orderedMap, outfile);
					orderedMap.clear();
					infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					email_num++;
					continue;
				}
				
			}

			// remove periods, commas, exclamation marks, question marks, and quotations
			
			word.erase(remove(word.begin(), word.end(), ','), word.end());
			word.erase(remove(word.begin(), word.end(), '!'), word.end());
			word.erase(remove(word.begin(), word.end(), '"'), word.end());
			word.erase(remove(word.begin(), word.end(), '.'), word.end());
			word.erase(remove(word.begin(), word.end(), '?'), word.end());
			word.erase(remove(word.begin(), word.end(), '~'), word.end());

			// transform word to lowcase characters

			transform(word.begin(), word.end(), word.begin(), ::tolower);

			if (orderedMap.count(word) > 0)
			{
				int freq_num = orderedMap[word];
				orderedMap[word] = freq_num + 1;
			}

			else {
				int freq_num = 1;
				orderedMap.insert(std::make_pair(word, freq_num));
			}
		}
	}

	if (email_num == 1) {
		printInfo(orderedMap, outfile);
	}

	return 0;
}
