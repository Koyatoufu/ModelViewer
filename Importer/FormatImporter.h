#pragma once
#include "ImportDefine.h"

class IBaseModelImporter;

class CFormatImporter
{
private:
	ModelData* (CFormatImporter::*m_parModelLoadFuntions[E_IMPORT_FORMAT_MAX])(std::basic_string<TCHAR> strFileName);
	IBaseModelImporter* m_parModelImpoter[E_IMPORT_FORMAT_MAX];
public:
	CFormatImporter();
	~CFormatImporter();

	ModelData* LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType);

private:
	ModelData* TxtLoad(std::basic_string<TCHAR> strFileName);
	ModelData* FbxLoad(std::basic_string<TCHAR> strFileName);
	ModelData* ObjLoad(std::basic_string<TCHAR> strFileName);
	ModelData* MD5Load(std::basic_string<TCHAR> strFileName);

	MaterialData* LoadMaterialObj(std::basic_string<TCHAR> strFileName, std::basic_string<TCHAR> strPath);

	std::basic_string<TCHAR> GetPathName(std::basic_string<TCHAR> strFileName);

};