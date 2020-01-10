#pragma once
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

