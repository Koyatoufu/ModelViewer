#pragma once
#include "ImportDefine.h"

class IBaseImporter
{
public:
	IBaseImporter() {}
	virtual ~IBaseImporter() {}

	virtual ModelData* ReadModel(std::basic_string<TCHAR> strFileName) = 0;

protected:
	virtual FILE* ReadFile(std::basic_string<TCHAR> strFileName) = 0;
private:
	IBaseImporter(IBaseImporter&) {}
};