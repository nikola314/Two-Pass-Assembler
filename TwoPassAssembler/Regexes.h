#pragma once
#include <regex>
class Regexes
{
public:
	Regexes();
	~Regexes();

	static std::regex LABEL;
	static std::regex DIRECTIVE;
	static std::regex INSTRUCTION;

	static std::regex SECTION;
	static std::regex TEXT;
	static std::regex DATA;
	static std::regex BSS;
	static std::regex END;
	static std::regex BYTE;
	static std::regex WORD;
	static std::regex ALIGN;
	static std::regex SKIP;
	static std::regex GLOBAL;
	static std::regex EXTERN;
	static std::regex EQU;

	static std::regex HALT;
	static std::regex XCHG;
	static std::regex INT;
	static std::regex MOV;
	static std::regex ADD;
	static std::regex SUB;
	static std::regex MUL;
	static std::regex DIV;
	static std::regex CMP;
	static std::regex NOT;
	static std::regex AND;
	static std::regex OR;
	static std::regex XOR;
	static std::regex TEST;
	static std::regex SHL;
	static std::regex SHR;
	static std::regex PUSH;
	static std::regex POP;
	static std::regex JMP;
	static std::regex JEQ;
	static std::regex JNE;
	static std::regex JGT;
	static std::regex CALL;
	static std::regex RET;
	static std::regex IRET;

	static std::regex IMMED;
	static std::regex IMMED_SYM;
	static std::regex REGDIR;
	static std::regex REGINDPOM;
	static std::regex REGINDPOM_SYM;
	static std::regex PCREL_SYM;
	static std::regex ABS;
	static std::regex ABS_SYM;
	static std::regex REGIND;

	static std::regex SYMBOL;
	
	static std::regex REGINDPOM_OFFSET;
	static std::regex IMMED_OPERAND;
	static std::regex MEMDIR_OPERAND;
};

