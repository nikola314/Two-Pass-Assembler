#include <cstdio>
#include <vector>
#include <string>
#include "Exceptions.h"
#include <fstream>
#include <string.h>

typedef vector<vector<string>> ParsedFile;
typedef vector<string> ParsedLine;

using namespace std;

// TESTING:
void printParsedFile(ParsedFile file) {
	for (int i = 0; i < file.size(); i++) {
		for (int j = 0; i < file[i].size(); j++) {
			printf(file[i][j].c_str());
			printf("\n");
		}
	}
}

ParsedFile parseInputFile(string filePath) {
	ParsedFile file;
	ifstream inFile(filePath);
	if (!inFile) {
		printf("Wrong input file passed");
		throw EXCEPTION_WRONG_INPUT_FILE;
	}
	string line;
	while (getline(inFile, line)) {
		if (!line.compare("\n") || !line.compare("") || line.at(0) == '@') continue;
		// Cut off the comment
		char *cleanLine = strtok(line.c_str, "@");
		// Split by [',',' ','\t','\r']
		char* token = strtok(cleanLine, ", \t\r");
		if (token == nullptr) continue;
		ParsedLine tokens;
		while (token && strcmp(token, ".end")) {
			// TODO: read the instruction and add it to file
			if (token[strlen(token) - 1] == ':') {
				// label
				ParsedLine label;
				label.push_back(token);
				file.push_back(label);
			}
			else {
				tokens.push_back(token);
			}
			token = strtok(nullptr, ", \t\r");
		}
		if (strcmp(token, ".end") == 0) break;
		if (tokens.size() > 0) file.push_back(tokens);
	}
	return file;
}


int main(int argc, char** argv)
{
	try {
		if (argc < 3) {
			printf("Invalid number of arguments!");
			throw EXCEPTION_INVALID_ARGUMENTS;
		}
		// Get indexes of input and output files
		int inputIndex = -1, outputIndex = -1;
		for (int i = 0; i < argc - 1; i++) {
			string str(argv[i]);
			if (str.compare("-o")) {
				outputIndex = i + 1;
				inputIndex = i == 0 ? 2 : 0;
			}
		}
		if (inputIndex < 0 || outputIndex < 0) {
			printf("Invalid arguments!");
			throw EXCEPTION_INVALID_ARGUMENTS;
		}

		string inputFileName(argv[inputIndex]);
		string outputFileName(argv[outputIndex]);

		ParsedFile inputFile = parseInputFile(inputFileName);
		// Test
		printParsedFile(inputFile);


		// TODO: Perform first and second pass over the inptu file

	}
	catch (int exceptionCode) {
		return exceptionCode;
	}
	return 0;
}