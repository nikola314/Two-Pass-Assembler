#include "TwoPassAssembler.h"
#include "definitions.h"
#include "Helpers.h"
#include <iostream>
#include <climits>
#include <vector>
#include <fstream>
#include <ostream>
#include <string>

using namespace std;

vector<string> TwoPassAssembler::errors;

TwoPassAssembler::TwoPassAssembler(ParsedFile file)
{
	this->inputFile = file;
}

void TwoPassAssembler::generateAssembly(string outputFilePath)
{
	bool noError = true;
	try {
		firstPass();
		secondPass();
	}
	catch (...) {
		noError = true;
	}
	if (noError) {
		std::ofstream file_out(outputFilePath);
		printToStream(file_out);
	}
	else {
		cout << "Error parsing file!";
	}
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
		currentSection->counter += Helpers::getValueFromNumberString(line.at(1));
	}
	if (type == DirectiveType::ALIGN) {
		currentSection->counter += Helpers::getValueFromNumberString(line.at(1)) - (currentSection->counter % Helpers::getValueFromNumberString(line.at(1)));
	}
	if (type == DirectiveType::EQU) {
		equMap[line.at(1)] = Helpers::getValueFromNumberString(line.at(2));
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
	for (ParsedLine line : inputFile) {
		string word = line.at(0);
		if (word == ".end") break;

		// replacing .equ defines (only those standing alone line vector
		for (int i = 1; i < line.size(); i++) {
			if (line.at(0) == ".equ") continue;
			if (equMap.find(line.at(i))!=equMap.end()) {
				line.at(i) = to_string(equMap[line.at(i)]);				
			}
		}

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
		currentSection->counter = 0;
	}
	if (type == DirectiveType::GLOBAL) {
		for (int i = 1; i < line.size(); i++) {
			Symbol* symbol = symbolTable.getSymbol(line.at(i));
			if (symbol != nullptr) {
				symbol->scope = SCOPE_GLOBAL;
			}
		}
	}
	if (type == DirectiveType::BYTE) {	
		for (int i = 1; i < line.size(); i++) {			
			int value = 0;
			if (Helpers::isSymbol(line.at(i))) {
				Symbol* symbol = symbolTable.getSymbol(line.at(i));
				if (symbol == nullptr) {
					symbolTable.insertSymbol(line.at(i), "?", Scope::SCOPE_GLOBAL, -1);
				}
				symbol = symbolTable.getSymbol(line.at(i));
				RelocationEntry* rel = new RelocationEntry(currentSection->counter, RelocationType::R_386_8, symbol->entryId);
				currentSection->relocationTable.push_back(rel);
			}
			else {
				value = Helpers::getValueFromNumberString(line.at(i));
			}		
			if (value > UCHAR_MAX) {
				errors.push_back("Unsupported value for .byte directive: " + value);
			}
			char data = (char)value;
			currentSection->data.push_back(data);
			currentSection->counter++;
		}
	}
	if (type == DirectiveType::WORD) {
		for (int i = 1; i < line.size(); i++) {
			int value = 0;
			if (Helpers::isSymbol(line.at(i))) {
				Symbol* symbol = symbolTable.getSymbol(line.at(i));
				if (symbol == nullptr) {
					symbolTable.insertSymbol(line.at(i), "?", Scope::SCOPE_GLOBAL, -1);
				}
				symbol = symbolTable.getSymbol(line.at(i));
				RelocationEntry* rel = new RelocationEntry(currentSection->counter, RelocationType::R_386_16, symbol->entryId);
				currentSection->relocationTable.push_back(rel);
			}
			else {
				value = Helpers::getValueFromNumberString(line.at(i));
			}
		
			if (value > USHRT_MAX) {
				errors.push_back("Unsupported value for .word directive: " + value);
			}
			short data = (short)value;
			char first = 0 | (data >> 8);
			char second = 0 | data;

			currentSection->data.push_back(second);
			currentSection->data.push_back(first);
							
			currentSection->counter += CPU_WORD;
		}
	}
	if (type == DirectiveType::SKIP) {
		int skip = Helpers::getValueFromNumberString(line.at(1));
		currentSection->counter += skip;
		for (int i = 0; i < skip; i++) currentSection->data.push_back(0);
	}
	if (type == DirectiveType::ALIGN) {	
		int bytesToFill = Helpers::getValueFromNumberString(line.at(1)) - (currentSection->counter % Helpers::getValueFromNumberString(line.at(1)));
		currentSection->counter += bytesToFill;
		for (int i = 0; i < bytesToFill; i++) currentSection->data.push_back(0);
	}
}

// TODO: REFACTOR !!!
void TwoPassAssembler::secondPassInstruction(ParsedLine line, InstructionType type)
{
	uint8_t instrDescr = type << 3;
	if (line.at(0).at(line.at(0).length() - 1) != 'b') {
		instrDescr |= 4;
	}
	currentSection->data.push_back(instrDescr);
	currentSection->counter++;
	

	if (type == InstructionType::IRET ||
		type == InstructionType::RET ||
		type == InstructionType::HALT) {
		// Nothing -> just instruction descriptor, no operands
	}
	else if (
		type == InstructionType::INT ||
		type == InstructionType::NOT ||
		type == InstructionType::POP ||
		type == InstructionType::JMP ||
		type == InstructionType::JEQ ||
		type == InstructionType::JNE ||
		type == InstructionType::JGT ||
		type == InstructionType::CALL ||
		type == InstructionType::PUSH
		) {
		secondPassInstructionOperand(line, 1);
	}
	else {
		secondPassInstructionOperand(line, 1);
		secondPassInstructionOperand(line, 2);
	}
}

void TwoPassAssembler::secondPassInstructionOperand(ParsedLine line, int operand)
{
	int operandLength = Helpers::getOperandLength(line);
	AddressMode addressMode = Helpers::getAddressMode(line.at(operand), operandLength);
	// Generate operand descriptor and if needed operand 1
	uint8_t opDesc = ((int)addressMode) << 5;

	currentSection->counter++; // for operator descriptor

	if (addressMode == AddressMode::A_REGDIR) {
		// r<val>
		opDesc |= Helpers::getRegisterBits(line.at(operand), 0);
		currentSection->data.push_back(opDesc);
		// No other operands
	}
	else if (addressMode == AddressMode::A_REGIND) {
		// [r<val>]
		opDesc |= Helpers::getRegisterBits(line.at(operand), 1);
		currentSection->data.push_back(opDesc);
		// No other operands
	}
	else if (addressMode == AddressMode::A_REGINDPOM_B) {
		opDesc |= Helpers::getRegisterBits(line.at(operand), 0);
		currentSection->data.push_back(opDesc);
		// r<num>[<val>], r<num>[<symbol_name>]
		// Get offset and push
		string offset = Helpers::getRegindpomOffset(line.at(operand));
		if (equMap.find(offset) != equMap.end()) {
			offset = to_string(equMap[offset]);
		}
		if (Helpers::isSymbol(offset)) {
			Symbol* symbol = symbolTable.getSymbol(offset);
			if (symbol == nullptr) {
				symbolTable.insertSymbol(offset, "?", Scope::SCOPE_GLOBAL, -1);
			}
			symbol = symbolTable.getSymbol(offset);
			RelocationEntry* rel = new RelocationEntry(currentSection->counter, RelocationType::R_386_8, symbol->entryId);
			currentSection->relocationTable.push_back(rel);
			currentSection->data.push_back(0);
		}
		else {
			int value = Helpers::getValueFromNumberString(offset);
			if (value > UCHAR_MAX) {
				errors.push_back("Unsupported operand for instruction: " + line.at(0));
			}
			char data = (char)value;
			currentSection->data.push_back(data);
		}
		currentSection->counter++; // first operand over			
	}
	else if (addressMode == AddressMode::A_REGINDPOM_W) {
		opDesc |= Helpers::getRegisterBits(line.at(operand), 0);
		currentSection->data.push_back(opDesc);
		// r<num>[<val>], r<num>[<symbol_name>], $<symbol_name>
		// Get offset and push
		string offset;
		AddressType at = Helpers::getAddressType(line.at(operand));
		if (at == AddressType::PCREL_SYM) {
			offset = Helpers::getMemdirOperand(line.at(operand));
		}
		else {
			offset = Helpers::getRegindpomOffset(line.at(operand));
		}
		if (equMap.find(offset) != equMap.end()) {
			offset = to_string(equMap[offset]);
		}
		if (Helpers::isSymbol(offset)) {
			Symbol* symbol = symbolTable.getSymbol(offset);
			if (symbol == nullptr) {
				symbolTable.insertSymbol(offset, "?", Scope::SCOPE_GLOBAL, -1);
			}
			symbol = symbolTable.getSymbol(offset);
			RelocationEntry* rel = new RelocationEntry(currentSection->counter, RelocationType::R_386_16, symbol->entryId);
			currentSection->relocationTable.push_back(rel);
			currentSection->data.push_back(0);
			currentSection->data.push_back(0);
		}
		else {
			int value = Helpers::getValueFromNumberString(offset);
			if (value > USHRT_MAX) {
				errors.push_back("Unsupported value for instruction: " + line.at(0));
			}
			short data = (short)value;
			char first = 0 | (data >> 8);
			char second = 0 | data;
			currentSection->data.push_back(second);
			currentSection->data.push_back(first);
		}
		currentSection->counter += CPU_WORD; // first operand over
	}
	else if (addressMode == AddressMode::A_IMMED) {
		// <val> and &<symbol_name>
		currentSection->data.push_back(opDesc);
		string oprnd = Helpers::getImmedOperand(line.at(operand));
		if (Helpers::isSymbol(oprnd)) {
			RelocationType relType = RelocationType::R_386_16;
			if (operandLength == 1) relType = RelocationType::R_386_8;
			Symbol* symbol = symbolTable.getSymbol(oprnd);
			if (symbol == nullptr) {
				symbolTable.insertSymbol(oprnd, "?", Scope::SCOPE_GLOBAL, -1);
			}
			symbol = symbolTable.getSymbol(oprnd);
			RelocationEntry* rel = new RelocationEntry(currentSection->counter, relType, symbol->entryId);
			currentSection->relocationTable.push_back(rel);
			for (int i = 0; i < operandLength; i++) {
				currentSection->data.push_back(0);
			}
		}
		else {
			int value = Helpers::getValueFromNumberString(oprnd);
			if (operandLength == 1) {
				if (value > UCHAR_MAX) {
					errors.push_back("Unsupported value for instruction: " + line.at(0));
				}
				char data = (char)value;
				currentSection->data.push_back(data);
			}
			else {
				if (value > USHRT_MAX) {
					errors.push_back("Unsupported value for instruction: " + line.at(0));
				}
				short data = (short)value;
				char first = 0 | (data >> 8);
				char second = 0 | data;
				currentSection->data.push_back(second);
				currentSection->data.push_back(first);
			}
		}
		currentSection->counter += operandLength;
	}
	else if (addressMode == AddressMode::A_MEMDIR) {
		// <symbol_name>, *<val>
	/*	if (Helpers::getAddressType(line.at(operand))== AddressType::PCREL_SYM) {
			opDesc|= Helpers::getRegisterBits(line.at(operand), 0);
		}*/
		currentSection->data.push_back(opDesc);
		string oprnd = Helpers::getMemdirOperand(line.at(operand));
		if (Helpers::isSymbol(oprnd)) {
			RelocationType relType = RelocationType::R_386_16;
			if (operandLength == 1) relType = RelocationType::R_386_8;
			Symbol* symbol = symbolTable.getSymbol(oprnd);
			if (symbol == nullptr) {
				symbolTable.insertSymbol(oprnd, "?", Scope::SCOPE_GLOBAL, -1);
			}
			symbol = symbolTable.getSymbol(oprnd);
			RelocationEntry* rel = new RelocationEntry(currentSection->counter, relType, symbol->entryId);
			currentSection->relocationTable.push_back(rel);
			for (int i = 0; i < operandLength; i++) {
				currentSection->data.push_back(0);
			}
		}
		else {
			int value = Helpers::getValueFromNumberString(oprnd);
			if (operandLength == 1) {
				if (value > UCHAR_MAX) {
					errors.push_back("Unsupported value for instruction: " + line.at(0));
				}
				char data = (char)value;
				currentSection->data.push_back(data);
			}
			else {
				if (value > USHRT_MAX) {
					errors.push_back("Unsupported value for instruction: " + line.at(0));
				}
				short data = (short)value;
				char first = 0 | (data >> 8);
				char second = 0 | data;
				currentSection->data.push_back(second);
				currentSection->data.push_back(first);				
			}
		}
		currentSection->counter += operandLength;
	}
}

void TwoPassAssembler::printSymbolTable(std::ostream& stream)
{
	stream << "SYMBOL TABLE:" << endl
		<< "LABEL\t\t" << "SECTION\t\t" << "OFFSET\t\t" << "SCOPE\t\t" << "ID" << endl;
	for (auto entry : symbolTable.table) {
		string offset = entry->offset != -1 ? to_string(entry->offset) : "?";
		string scope = entry->scope == Scope::SCOPE_LOCAL ? "l" : "g";
		stream << entry->name << "\t\t" << entry->section << "\t\t" 
			<< offset << "\t\t" << scope << "\t\t" << entry->entryId << endl;
	}
}

void TwoPassAssembler::printErrors(std::ostream& stream)
{
	if (errors.size() == 0) return;
	stream << endl << endl << "ERRORS:" << endl;
	for (auto error : errors) {
		stream << error << endl;
	}
}

void TwoPassAssembler::printSections(std::ostream& stream)
{
	stream << endl << endl << "SECTIONS:" << endl
		<< "NAME\t\t" << "FLAGS\t\t" << "LOCATION COUNTER" << endl;
	for (auto section : sections) {
		stream << section->name << "\t\t" << section->flags << "\t\t" << section->counter << endl;
	}
}

void TwoPassAssembler::printSectionData(std::ostream& stream)
{
	for (auto section : sections) {
		stream << endl << endl << "SECTION: " << section->name << endl;
		int cnt = 0;
		for (auto byte : section->data) {
			stream << Helpers::getHexStringFromByte(byte) << " ";
			cnt++;
			if (cnt % 16 == 0) {
				stream << endl;
			}
		}
	}
}

void TwoPassAssembler::printRelocationTables(std::ostream& stream)
{
	for (auto section : sections) {
		stream << endl << endl << "Relocation table - " + section->name << endl;
		stream << "OFFSET\t\tTYPE\t\t\tSYM.ID." << endl;
		for (auto entry : section->relocationTable) {
			string type = entry->type == RelocationType::R_386_8 ? "R_386_8 " : "R_386_16";
			stream << entry->offset << "\t\t" << type << "\t\t" << entry->symbolId << endl;
		}
	}
}

void TwoPassAssembler::printToStream(std::ostream & stream)
{
	printSymbolTable(stream);
	printSections(stream);
	printSectionData(stream);
	printRelocationTables(stream);
	printErrors(stream);
}
