#ifndef __HELPERS__H
#define __HELPERS__H 1
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
	static uint8_t getRegisterBits(std::string regString, int offset);
	static bool isSymbol(std::string);
	static std::string getRegindpomOffset(std::string operand);
	static std::string getImmedOperand(std::string operand);
	static std::string getMemdirOperand(std::string operand);
	static std::string getHexStringFromByte(char byte);
	static std::string getHexChar(char);
	static int getValueFromNumberString(std::string num);
};

#endif