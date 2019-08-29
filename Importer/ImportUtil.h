#pragma once
#include "ImportDefine.h"
#include "SingleTon.h"

class CFormatImporter;

class CImportUtil:public CSingleTonT<CImportUtil>
{
	friend CSingleTonT;

	CFormatImporter* m_pModelImporter;
protected:
	CImportUtil();
	virtual ~CImportUtil();
public:
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);

	ModelData* LoadModelData(std::basic_string<TCHAR> strFileName);

};