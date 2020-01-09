#include "TwoPassAssembler.h"
#include "definitions.h"
#include "Helpers.h"
#include <iostream>

using namespace std;

vector<string> TwoPassAssembler::errors;

TwoPassAssembler::TwoPassAssembler(ParsedFile file)
{
	this->inputFile = file;
}

void TwoPassAssembler::generateAssembly(string outputFilePath)
{
	firstPass();
	secondPass();
	writeToOutputFile(outputFilePath);
}


TwoPassAssembler::~TwoPassAssembler()
{
}

void TwoPassAssembler::firstPass()
{
	for (ParsedLine line : inputFile) {
		string word = line.at(0);
		if (word == ".end") break;
		WordType type = Helpers::getType(word);
		firstPass(line,type);
	}
}

void TwoPassAssembler::firstPass(ParsedLine line, WordType type) {
	string directive = line.at(0);
	switch (type) {
	case WordType::LABEL:
		firstPassLabel(line);
		break;
	case WordType::INSTRUCTION:
		firstPassInstruction(line, Helpers::getInstructionType(directive));
		break;
	case WordType::DIRECTIVE:
		DirectiveType directiveType = Helpers::getDirectiveType(directive);
		firstPassDirective(line, directiveType);
		break;
	}
}

void TwoPassAssembler::firstPassDirective(ParsedLine line, DirectiveType type)
{
	if (
		type == DirectiveType::TEXT ||
		type == DirectiveType::BSS ||
		type == DirectiveType::DATA ||
		type == DirectiveType::SECTION
		) {
		string sectionName = line.at(0);
		string flags = "";
		if (type == DirectiveType::SECTION) {
			sectionName = line.at(1);
			flags = line.at(2);
			flags = flags.substr(1, flags.length() - 2);
		}

		if (sectionExists(sectionName)) {
			errors.push_back("Section " + sectionName + " declared multiple times");
		}
		Section* section = getSection(sectionName, flags);
		currentSection = section;
		symbolTable.insertSymbol(sectionName, sectionName, Scope::SCOPE_LOCAL, section->counter);
	}
	
	if (type == DirectiveType::BYTE) {
		currentSection->counter+= line.size()-1;
	}
	if (type == DirectiveType::WORD) {
		currentSection->counter += CPU_WORD * (line.size() - 1);
	}
	if (type == DirectiveType::SKIP) {
		currentSection->counter += stoi(line.at(1));
	}
	if (type == DirectiveType::ALIGN) {
		currentSection->counter += stoi(line.at(1)) - (currentSection->counter % stoi(line.at(1)));
	}
	if (type == DirectiveType::EQU) {
		equMap[line.at(1)] = stoi(line.at(2));
	}
}

void TwoPassAssembler::firstPassInstruction(ParsedLine line, InstructionType type)
{
	string instruction = line.at(0);
	int instructionLength = 0;
	instructionLength = Helpers::getInstructionLength(line, type);

	currentSection->counter += instructionLength;
}

void TwoPassAssembler::firstPassLabel(ParsedLine line)
{
	string label = line.at(0);
	label = label.substr(0, label.length()-1); // cut ":" from the end

	if (symbolTable.isDuplicateEntry(label)) {
		errors.push_back("Duplicate label: " + label);
		return;
	}
	symbolTable.insertSymbol(label, currentSection->name, Scope::SCOPE_LOCAL, currentSection->counter);
}

Section * TwoPassAssembler::getSection(string name, string flags)
{
	Section * retval = nullptr;
	for (Section* section : sections) {
		if (section->name == name) {
			retval = section;
		}
	}
	if (retval == nullptr) {
		retval = new Section(name, flags);
		sections.push_back(retval);
	}
	return retval;
}

bool TwoPassAssembler::sectionExists(string name) {
	for (Section* section : sections) {
		if (section->name == name) {
			return true;
		}
	}
	return false;
}

void TwoPassAssembler::secondPass()
{
	currentSection = nullptr;
	// TODO: Set location counters of sections to 0
	for (ParsedLine line : inputFile) {
		string word = line.at(0);
		if (word == ".end") break;
		WordType type = Helpers::getType(word);
		secondPass(line, type);
	}
}

void TwoPassAssembler::secondPass(ParsedLine line, WordType type)
{
	string directive = line.at(0);
	switch (type) {
	case WordType::INSTRUCTION:
		secondPassInstruction(line, Helpers::getInstructionType(directive));
		break;
	case WordType::DIRECTIVE:
		DirectiveType directiveType = Helpers::getDirectiveType(directive);
		secondPassDirective(line, directiveType);
		break;
	}
}

void TwoPassAssembler::secondPassDirective(ParsedLine line, DirectiveType type)
{
	if(
		type == DirectiveType::TEXT ||
		type == DirectiveType::BSS ||
		type == DirectiveType::DATA ||
		type == DirectiveType::SECTION
	) {
		int index = 0;
		if (type == DirectiveType::SECTION) {
			index = 1;
		}
		string sectionName = line.at(index);
		currentSection = getSection(sectionName);
	}
	if (type == DirectiveType::GLOBAL) {
		// set scope to global
	}
	if (type == DirectiveType::BYTE) {
		// generate data
	}
	if (type == DirectiveType::WORD) {
		// generate data
	}
	if (type == DirectiveType::SKIP) {
		// generate data
	}
	if (type == DirectiveType::ALIGN) {
		// generate data
	}
}

void TwoPassAssembler::secondPassInstruction(ParsedLine, InstructionType)
{
}

void TwoPassAssembler::writeToOutputFile(std::string filePath)
{
}

void TwoPassAssembler::printSymbolTable()
{
	cout << endl << endl << "SYMBOL TABLE:" << endl
		<< "LABEL\t\t" << "SECTION\t\t" << "OFFSET\t\t" << "SCOPE\t\t" << "ID" << endl;
	for (auto entry : symbolTable.table) {
		cout << entry->name << "\t\t" << entry->section << "\t\t" 
			<< entry->offset << "\t\t" << entry->scope << "\t\t" << entry->entryId << endl;
	}
}

void TwoPassAssembler::printErrors()
{
	cout << endl << endl << "ERRORS:" << endl;
	for (auto error : errors) {
		cout << error << endl;
	}
}

void TwoPassAssembler::printSections()
{
	cout << endl << endl << "SECTIONS:" << endl
		<< "NAME\t\t" << "FLAGS\t\t" << "COUNTER" << endl;
	for (auto section : sections) {
		cout << section->name << "\t\t" << section->flags << "\t\t" << section->counter << endl;
	}
}
