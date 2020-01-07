#pragma once
#include "definitions.h"
#include "Section.h"
#include "SymbolTable.h"

class TwoPassAssembler
{
public:
	TwoPassAssembler(ParsedFile file);

	void generateAssembly(std::string outputFilePath);

	~TwoPassAssembler();

private:
	unsigned int locationCounter;

	ParsedFile inputFile;
	std::vector<Section*> sections;
	Section* currentSection;
	SymbolTable symbolTable;
	
	void firstPass();
	void firstPass(ParsedLine line, WordType type);
	void firstPassDirective(ParsedLine, DirectiveType);
	void firstPassInstruction(ParsedLine, InstructionType);
	void firstPassLabel(ParsedLine);
	Section* getSection(string name, string flags);

	void secondPass();
	void writeToOutputFile(std::string filePath);
};

