#ifndef __ASSEMBLER__H
#define __ASSEMBLER__H 1
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
	std::map<string, int> equMap;
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
	void secondPassInstructionOperand(ParsedLine line, int operand);

	Section* getSection(string name, string flags = "");
	bool sectionExists(string name);
	void printSymbolTable(std::ostream&);
	void printErrors(std::ostream&);
	void printSections(std::ostream&);
	void printSectionData(std::ostream&);
	void printRelocationTables(std::ostream&);

	/*	TESTING	*/
public:

	void printToStream(std::ostream&);
};

#endif