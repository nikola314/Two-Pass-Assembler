#pragma once
#include <vector>
#include <string>

class Section
{
	std::string name;
	std::string flags;	// For [, "<flags>"] after .section
	std::vector<unsigned char> data; 
	unsigned int counter;

	friend class TwoPassAssembler;
public:
	Section(std::string, std::string);
	~Section();
};

