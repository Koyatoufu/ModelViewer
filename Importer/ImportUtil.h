#pragma once
#include "ImportDefine.h"
#include "SingleTon.h"

class CModelImporter;

class CImportUtil:public CSingleTonT<CImportUtil>
{
	friend CSingleTonT;

	CModelImporter* m_pModelImporter;
protected:
	CImportUtil();
	virtual ~CImportUtil();
public:
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);

	ModelData* LoadModelData(std::basic_string<TCHAR> strFileName);

};