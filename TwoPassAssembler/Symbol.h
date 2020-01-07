#pragma once
#include<string>
#include"definitions.h"

class Symbol
{
public:
	std::string name;
	std::string section;
	Scope scope;
	unsigned int offset;
	unsigned int entryId;

	Symbol(std::string name, std::string section, Scope scope, unsigned int offset, unsigned int entryId);
	~Symbol();

};

