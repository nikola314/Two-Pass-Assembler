#include "Symbol.h"


Symbol::Symbol(std::string name, std::string section, Scope scope, unsigned int offset, unsigned int entryId)
{
	this->name = name;
	this->section = section;
	this->scope = scope;
	this->offset = offset;
	this->entryId = entryId;
}


Symbol::~Symbol()
{
}

