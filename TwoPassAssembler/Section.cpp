#include "Section.h"

using namespace std;

Section::Section(string name, string flags="")
{
	counter = 0;
	this->name = name;
	this->flags = flags;
}


Section::~Section()
{
}
