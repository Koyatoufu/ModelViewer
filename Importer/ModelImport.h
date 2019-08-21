#pragma once
#include "ImportDefine.h"

namespace ImportUtil
{
	namespace Model
	{
		static ModelData* (*pModelLoadFuntions[E_IMPORT_FORMAT_MAX])(std::basic_string<TCHAR> strFileName);

		void SetFunctionPointSet();

		ModelData* TxtLoad(std::basic_string<TCHAR> strFileName);
		ModelData* FbxLoad(std::basic_string<TCHAR> strFileName);
		ModelData* ObjLoad(std::basic_string<TCHAR> strFileName);
		ModelData* MD5Load(std::basic_string<TCHAR> strFileName);

	}
}

