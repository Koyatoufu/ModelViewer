#pragma once
#include "ImportDefine.h"
#include "SingleTon.h"

class CImportUtil:public CSingleTonT<CImportUtil>
{
	friend CSingleTonT;
protected:
	CImportUtil();
	virtual ~CImportUtil();
public:
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);

	ModelData* LoadModelData(std::basic_string<TCHAR> strFileName);

};