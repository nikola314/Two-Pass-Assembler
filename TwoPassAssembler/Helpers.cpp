#include "Helpers.h"
#include "Regexes.h"
#include <regex>

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


	return AddressType();
}

int Helpers::getOperandLength(ParsedLine line)
{
	string instruction = line.at(0);
	int operandLength = 2;
	char lastChar = instruction.at(instruction.length() - 1);
	if (lastChar == 'b') {
		operandLength = 1;
	}
	return operandLength;
}

