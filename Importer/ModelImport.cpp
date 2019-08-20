#include "ModelImport.h"

void ImporterUtil::Model::SetFunctionPointSet()
{
	pModelLoadFuntions[E_IMPORT_FORMAT_TXT] = TxtLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = ObjLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_FBX] = FbxLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_MD5] = MD5Load;
}

ModelData * ImporterUtil::Model::TxtLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImporterUtil::Model::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImporterUtil::Model::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImporterUtil::Model::MD5Load(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}
