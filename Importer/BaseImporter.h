#pragma once
#include "ImportDefine.h"

class CBaseImporter
{
public:
	CBaseImporter();
	virtual ~CBaseImporter();

	virtual ModelData* ReadFile();


protected:
	CBaseImporter(CBaseImporter&) {}
};