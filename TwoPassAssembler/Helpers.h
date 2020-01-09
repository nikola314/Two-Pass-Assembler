#pragma once
#include <string>
#include <regex>
#include "definitions.h"

class Helpers
{
public:
	Helpers();
	~Helpers();
	static WordType getType(std::string value);
	static DirectiveType getDirectiveType(std::string directive);
	static InstructionType getInstructionType(std::string instruction);
	static AddressType getAddressType(std::string address);
	static AddressMode getAddressMode(std::string address, int operandLength);
	static int getInstructionLength(ParsedLine line, InstructionType type);
	static int getOperandLength(ParsedLine line);
};

