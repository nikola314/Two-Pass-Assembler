#ifndef __SECTION__H
#define __SECTION__H 1
#include <vector>
#include <string>
#include "RelocationEntry.h"

class Section
{
	std::string name;
	std::string flags;	// For [, "<flags>"] after .section
	std::vector<char> data; 
	unsigned int counter;
	std::vector<RelocationEntry*> relocationTable;

	friend class TwoPassAssembler;
public:
	Section(std::string, std::string);
	~Section();
};

#endif