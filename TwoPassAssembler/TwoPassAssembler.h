#pragma once
#include "definitions.h"
#include "Section.h"
#include "SymbolTable.h"
#include<map>

class TwoPassAssembler
{
public:
	TwoPassAssembler(ParsedFile file);
	~TwoPassAssembler();
	void generateAssembly(std::string outputFilePath);
	static std::vector<string> errors;

private:
	unsigned int locationCounter;

	ParsedFile inputFile;
	std::vector<Section*> sections;	
	std::map<string, unsigned int> equMap;
	Section* currentSection = nullptr;
	SymbolTable symbolTable;
	
	void firstPass();
	void firstPass(ParsedLine line, WordType type);
	void firstPassDirective(ParsedLine line, DirectiveType type);
	void firstPassInstruction(ParsedLine line, InstructionType type);
	void firstPassLabel(ParsedLine line);

	void secondPass();
	void secondPass(ParsedLine line, WordType type);
	void secondPassDirective(ParsedLine line, DirectiveType type);
	void secondPassInstruction(ParsedLine line, InstructionType type);

	Section* getSection(string name, string flags = "");
	bool sectionExists(string name);
	void writeToOutputFile(std::string filePath);

	/*	TESTING	*/
public:
	void printSymbolTable();
	void printErrors();
	void printSections();
};

