#pragma once
#include "ImportDefine.h"

	class CModelImporter
	{
	private:
		ModelData* (*pModelLoadFuntions[E_IMPORT_FORMAT_MAX])(std::basic_string<TCHAR> strFileName);
	public:
		CModelImporter();
		~CModelImporter();

		void SetFunctionPointSet();

		ModelData* LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType);

		ModelData* TxtLoad(std::basic_string<TCHAR> strFileName);
		ModelData* FbxLoad(std::basic_string<TCHAR> strFileName);
		ModelData* ObjLoad(std::basic_string<TCHAR> strFileName);
		ModelData* MD5Load(std::basic_string<TCHAR> strFileName);

	};