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

};

