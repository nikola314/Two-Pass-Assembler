#include <cstdio>
#include <vector>
#include <string>
#include "definitions.h" 
#include <fstream>
#include <string.h>
#include "TwoPassAssembler.h"
#include "Symbol.h"

using namespace std;

typedef vector<string> ParsedLine;
typedef vector<ParsedLine> ParsedFile;

/******************		TESTING		***********************/
void printParsedFile(ParsedFile file) {
	for (int i = 0; i < file.size(); i++) {
		for (int j = 0; j < file[i].size(); j++) {
			printf(file[i][j].c_str());
			printf(" ");
		}
		printf("\n");
	}
}

/*********************************************************/


ParsedFile parseInputFile(string filePath) {
	ParsedFile file;
	ifstream inFile(filePath);
	if (!inFile) {
		printf("Wrong input file passed");
		throw EXCEPTION_WRONG_INPUT_FILE;
	}
	char * nextToken = NULL;
	string line;
	char lineptr[255];
	while (getline(inFile, line)) {
		if (!line.compare("\n") || !line.compare("") || line.at(0) == '@') continue;
		// Cut off the comment
		strcpy_s(lineptr, line.c_str());
		char *cleanLine = strtok_s(lineptr, "@", &nextToken);
		nextToken = NULL;
		// Split by [',',' ','\t','\r']
		char* token = strtok_s(cleanLine, ", \t\r", &nextToken);
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
			token = strtok_s(nullptr, ", \t\r", &nextToken);
		}
		if (token != NULL) {
			if (strcmp(token, ".end") == 0) break;
		}
		if (tokens.size() > 0) file.push_back(tokens);
	}
	inFile.close();
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
		for (int i = 1; i < argc - 1; i++) {
			string str(argv[i]);
			if (str.compare("-o") == 0) {
				outputIndex = i + 1;
				inputIndex = i == 1 ? 3 : 1;
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
		getchar();

		TwoPassAssembler assembler(inputFile);
		assembler.generateAssembly(outputFileName);

		// Test
		assembler.printSymbolTable();
		assembler.printSections();
		assembler.printErrors();
		getchar();
	}
	catch (int exceptionCode) {
		// TODO: print the exception
		return exceptionCode;
	}
	return 0;
}