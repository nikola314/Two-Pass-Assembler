#include "RelocationEntry.h"


RelocationEntry::RelocationEntry(int offset, RelocationType type, int symbolId)
{
	this->offset = offset;
	this->type = type;
	this->symbolId = symbolId;
}

RelocationEntry::~RelocationEntry()
{
}
