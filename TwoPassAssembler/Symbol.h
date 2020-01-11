#ifndef __SYMBOL__H
#define __SYMBOL__H 1
#include<string>
#include"definitions.h"

class Symbol
{
public:
	std::string name;
	std::string section;
	Scope scope;
	int offset;
	unsigned int entryId;

	Symbol(std::string name, std::string section, Scope scope, unsigned int offset, unsigned int entryId);
	~Symbol();

};

#endif