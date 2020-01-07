#include "SymbolTable.h"

unsigned int SymbolTable::lastEntryId = 1;

SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

void SymbolTable::insertSymbol(std::string name, std::string section, Scope scope, unsigned int offset)
{
	// TODO: implement
}

bool SymbolTable::isDuplicateEntry(std::string name)
{
	for (auto const& symbol : table) {
		if (symbol.name == name) return true;
	}
	return false;
}

Symbol* SymbolTable::getSymbol(std::string name)
{
	for (auto symbol : table) {
		if (symbol.name == name) return &symbol;
	}
	return nullptr;
}
