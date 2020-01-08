#pragma once
#include<vector>
#include "definitions.h"
#include "Symbol.h"

using namespace std;

class SymbolTable
{
private:
	vector<Symbol*> table;
	static unsigned int lastEntryId;
public:
	SymbolTable();
	~SymbolTable();
	void insertSymbol(std::string name, std::string section, Scope scope, unsigned int offset);
	bool isDuplicateEntry(std::string name);
	Symbol* getSymbol(std::string name);

	/*	TESTING	*/
	friend class TwoPassAssembler;
};
