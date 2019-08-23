#pragma once
#include "ImportDefine.h"

class IBaseModelImporter;

class CModelImporter
{
private:
	ModelData* (CModelImporter::*m_parModelLoadFuntions[E_IMPORT_FORMAT_MAX])(std::basic_string<TCHAR> strFileName);
	IBaseModelImporter* m_parModelImpoter[E_IMPORT_FORMAT_MAX];
public:
	CModelImporter();
	~CModelImporter();

	ModelData* LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType);

	ModelData* TxtLoad(std::basic_string<TCHAR> strFileName);
	ModelData* FbxLoad(std::basic_string<TCHAR> strFileName);
	ModelData* ObjLoad(std::basic_string<TCHAR> strFileName);
	ModelData* MD5Load(std::basic_string<TCHAR> strFileName);

};