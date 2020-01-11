#ifndef __RELOCATION_ENTRY__H
#define __RELOCATION_ENTRY__H 1
#include "definitions.h"
class RelocationEntry
{
	RelocationType type;
	int offset;
	int symbolId;

	friend class TwoPassAssembler;
public:
	RelocationEntry(int offset, RelocationType type, int symbolId);
	~RelocationEntry();
};

#endif