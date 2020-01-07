#include "TwoPassAssembler.h"
#include "definitions.h"
#include "Helpers.h"

using namespace std;

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
		switch (directiveType) {

		}
		break;
	}
}

void TwoPassAssembler::firstPassDirective(ParsedLine, DirectiveType)
{

}

void TwoPassAssembler::firstPassInstruction(ParsedLine, InstructionType)
{

}

void TwoPassAssembler::firstPassLabel(ParsedLine)
{

}

Section * TwoPassAssembler::getSection(string name, string flags = "")
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

void TwoPassAssembler::secondPass()
{
}

void TwoPassAssembler::writeToOutputFile(std::string filePath)
{
}
