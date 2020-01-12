#include "Helpers.h"
#include "Regexes.h"
#include <regex>
#include <string>
#include "TwoPassAssembler.h"

using namespace std;

Helpers::Helpers()
{
}


Helpers::~Helpers()
{
}

WordType Helpers::getType(std::string value)
{
	if (regex_match(value, Regexes::LABEL)) return LABEL;
	if (regex_match(value, Regexes::DIRECTIVE)) return DIRECTIVE;
	if (regex_match(value, Regexes::INSTRUCTION)) return INSTRUCTION;

	TwoPassAssembler::errors.push_back("Can't parse input file!");
	return WordType();
}

DirectiveType Helpers::getDirectiveType(std::string directive)
{
	if (regex_match(directive, Regexes::SECTION)) return SECTION;
	if (regex_match(directive, Regexes::TEXT)) return TEXT;
	if (regex_match(directive, Regexes::DATA)) return DATA;
	if (regex_match(directive, Regexes::BSS)) return BSS;
	if (regex_match(directive, Regexes::END)) return END;
	if (regex_match(directive, Regexes::BYTE)) return BYTE;
	if (regex_match(directive, Regexes::WORD)) return WORD;
	if (regex_match(directive, Regexes::ALIGN)) return ALIGN;
	if (regex_match(directive, Regexes::SKIP)) return SKIP;
	if (regex_match(directive, Regexes::GLOBAL)) return GLOBAL;
	if (regex_match(directive, Regexes::EXTERN)) return EXTERN;
	if (regex_match(directive, Regexes::EQU)) return EQU;

	TwoPassAssembler::errors.push_back("Can't parse directive: "+directive);
	return DirectiveType();
}

InstructionType Helpers::getInstructionType(std::string instruction)
{
	if (regex_match(instruction, Regexes::HALT)) return HALT;
	if (regex_match(instruction, Regexes::XCHG)) return XCHG;
	if (regex_match(instruction, Regexes::INT)) return INT;
	if (regex_match(instruction, Regexes::MOV)) return MOV;
	if (regex_match(instruction, Regexes::ADD)) return ADD;
	if (regex_match(instruction, Regexes::SUB)) return SUB;
	if (regex_match(instruction, Regexes::MUL)) return MUL;
	if (regex_match(instruction, Regexes::DIV)) return DIV;
	if (regex_match(instruction, Regexes::CMP)) return CMP;
	if (regex_match(instruction, Regexes::NOT)) return NOT;
	if (regex_match(instruction, Regexes::AND)) return AND;
	if (regex_match(instruction, Regexes::OR)) return OR;
	if (regex_match(instruction, Regexes::XOR)) return XOR;
	if (regex_match(instruction, Regexes::TEST)) return TEST;
	if (regex_match(instruction, Regexes::SHL)) return SHL;
	if (regex_match(instruction, Regexes::SHR)) return SHR;
	if (regex_match(instruction, Regexes::PUSH)) return PUSH;
	if (regex_match(instruction, Regexes::POP)) return POP;
	if (regex_match(instruction, Regexes::JMP)) return JMP;
	if (regex_match(instruction, Regexes::JEQ)) return JEQ;
	if (regex_match(instruction, Regexes::JNE)) return JNE;
	if (regex_match(instruction, Regexes::JGT)) return JGT;
	if (regex_match(instruction, Regexes::CALL)) return CALL;
	if (regex_match(instruction, Regexes::RET)) return RET;
	if (regex_match(instruction, Regexes::IRET)) return IRET;

	TwoPassAssembler::errors.push_back("Can't parse instruction: " + instruction);
	return InstructionType();
}

AddressType Helpers::getAddressType(std::string address)
{
	if (regex_match(address, Regexes::IMMED)) return IMMED;
	if (regex_match(address, Regexes::IMMED_SYM)) return IMMED_SYM;
	if (regex_match(address, Regexes::REGDIR)) return REGDIR;
	if (regex_match(address, Regexes::REGINDPOM)) return REGINDPOM;
	if (regex_match(address, Regexes::REGINDPOM_SYM)) return REGINDPOM_SYM;
	if (regex_match(address, Regexes::PCREL_SYM)) return PCREL_SYM;
	if (regex_match(address, Regexes::ABS)) return ABS;
	if (regex_match(address, Regexes::ABS_SYM)) return ABS_SYM;
	if (regex_match(address, Regexes::REGIND)) return REGIND;

	TwoPassAssembler::errors.push_back("Can't parse addressing: " + address);
	return AddressType();
}

AddressMode Helpers::getAddressMode(std::string address, int operandLength)
{
	AddressType type = getAddressType(address);
	if (type == AddressType::IMMED ||
		type == AddressType::IMMED_SYM) {
		return AddressMode::A_IMMED;
	}
	if (type == AddressType::ABS ||
		type == AddressType::ABS_SYM) {
		return AddressMode::A_MEMDIR;
	}
	if (type == AddressType::REGINDPOM ||
		type == AddressType::REGINDPOM_SYM) {
		if (operandLength == 1) {
			return AddressMode::A_REGINDPOM_B;
		}
		return AddressMode::A_REGINDPOM_W;
	}
	if (type == AddressType::PCREL_SYM) {
		return AddressMode::A_REGINDPOM_W;
	}
	if (type == AddressType::REGIND) {
		return AddressMode::A_REGIND;
	}
	if (type == AddressType::REGDIR) {
		return AddressMode::A_REGDIR;
	}
	TwoPassAssembler::errors.push_back("Can't determine address mode!");
	return AddressMode();
}

int Helpers::getInstructionLength(ParsedLine line, InstructionType type)
{
	int instructionLength = 1;
	if (	// 0 operands
		type == InstructionType::RET ||
		type == InstructionType::IRET ||
		type == InstructionType::HALT
		) {
		// Nothing -> instructionLength = 1
	}
	else if (	// 1 operand -> 1st is dst
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
		int operandLength = getOperandLength(line);
		AddressMode addressMode = getAddressMode(line.at(1),operandLength);
		if (addressMode == AddressMode::A_IMMED && type != InstructionType::PUSH) {
			TwoPassAssembler::errors.push_back("Immed in dst operand!");
		}
		if (addressMode != AddressMode::A_REGDIR) {
			instructionLength += operandLength; // Register declared in descriptor -> no additional bytes
		}
		instructionLength += 1; // Operand descriptor		
	}
	else {	// 2 operands
		int operandLength = getOperandLength(line);
		instructionLength += 2; // 2 operand descriptors
		AddressMode addressMode = getAddressMode(line.at(1), operandLength); // First operand
		if (addressMode != AddressMode::A_REGDIR) {
			instructionLength += operandLength;
		}
		addressMode = getAddressMode(line.at(2), operandLength); // Second operand
		if (addressMode != AddressMode::A_REGDIR) {
			instructionLength += operandLength;
		}
	}

	return instructionLength;
}

int Helpers::getOperandLength(ParsedLine line)
{
	string instruction = line.at(0);
	int operandLength = 2;
	char lastChar = instruction.at(instruction.length() - 1);
	if (lastChar == 'b' && instruction.length()>3) {
		operandLength = 1;
	}

	return operandLength;
}

uint8_t Helpers::getRegisterBits(std::string regString, int offset)
{
	if (regString.rfind("sp", offset) == offset) {
		return SP << 1;
	}
	if ((regString.rfind("pc", offset) == offset) || regString.at(0)=='$')  {
		return PC << 1;
	}

	char regNumChar = regString.at(offset+1);
	int regNum = regNumChar - '0';
	regNum = regNum << 1;
	if (regString.length() > offset+2 && regString.at(offset+2) == 'h') {
		regNum |= 1;
	}

	return regNum;
}

bool Helpers::isSymbol(std::string value)
{
	if (regex_match(value, Regexes::SYMBOL)) {
		return true;
	}
	return false;
}

string Helpers::getRegindpomOffset(std::string operand)
{
	smatch matches;
	if (regex_search(operand, matches, Regexes::REGINDPOM_OFFSET)) {

		return matches[3].str();
	}
	else {
		TwoPassAssembler::errors.push_back("Wrong addressing: " + operand);
	}

	return string();
}

std::string Helpers::getImmedOperand(std::string operand)
{
	smatch matches;
	if (regex_search(operand, matches, Regexes::IMMED_OPERAND)) {
		string match = matches[1].str();
		if (match.at(0) == '&') {
			return match.substr(1, match.length());
		}
		return match;
	}
	else {
		TwoPassAssembler::errors.push_back("Wrong addressing: " + operand);
	}
	return std::string();
}

std::string Helpers::getMemdirOperand(std::string operand)
{
	smatch matches;
	if (regex_search(operand, matches, Regexes::MEMDIR_OPERAND)) {
		string match = matches[1].str();
		if (match.at(0) == '$' || match.at(0)=='*') {
			return match.substr(1, match.length());
		}
		return match;
	}
	else {
		TwoPassAssembler::errors.push_back("Wrong addressing: " + operand);
	}
	return std::string();
}

std::string Helpers::getHexStringFromByte(char byte)
{
	char firstVal = (byte >> 4) & 15;
	char secondVal = byte & 15;
	string retval = "";
	retval.append(getHexChar(firstVal));
	retval.append(getHexChar(secondVal));
	return retval;
}

string Helpers::getHexChar(char value) {
	string retval = "x";
	if (value < 10) {
		retval.at(0) = '0' + value;
		return retval;
	}
	switch (value) {
		case 10: retval.at(0) = 'a'; break;
		case 11: retval.at(0) = 'b'; break;
		case 12: retval.at(0) = 'c'; break;
		case 13: retval.at(0) = 'd'; break;
		case 14: retval.at(0) = 'e'; break;
		case 15: retval.at(0) = 'f'; break;
	}
	return retval;
}

int Helpers::getValueFromNumberString(std::string num)
{
	int radix = 10;
	if (num.length() > 1) {
		// TODO: match with regex instead of this
		if (num.at(1) == 'x') {
			radix = 16;
		}
		else if (num.at(1) == 'b') {
			radix = 2;
		}
	}
	return stoi(num, 0, radix);
}


