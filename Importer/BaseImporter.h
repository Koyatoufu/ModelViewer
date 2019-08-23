#pragma once
#include "ImportDefine.h"

class IBaseImporter
{
public:
	IBaseImporter() {}
	virtual ~IBaseImporter() {}

	virtual bool CanReadFile() = 0;
	virtual ModelData* ReadFile() = 0;


private:
	IBaseImporter(IBaseImporter&) {}
};