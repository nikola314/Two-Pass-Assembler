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

	return DirectiveType();
}

InstructionType Helpers::getInstructionType(std::string instruction)
{
	return InstructionType();
}
