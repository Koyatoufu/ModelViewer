#include "MaterialImport.h"

MaterialData * ImportUtil::LoadMtlFormat(std::basic_string<TCHAR> strFileName)
{
	if (strFileName.empty())
		return nullptr;

	std::ifstream fin;

	return nullptr;
}

MaterialData * ImportUtil::CreateMaterialData(int nCount, std::vector<MaterialData::MaterialInfo*> vtMaterialInfo)
{
	if (nCount < 1)
		return nullptr;

	MaterialData* pData = new MaterialData();

	pData->vtMaterialInfo = vtMaterialInfo;

	return pData;
}

MaterialData::MaterialInfo* ImportUtil::CreateMaterialInfo( std::basic_string<TCHAR> strName,
	DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 specular,
	std::basic_string<TCHAR> strDiffuse, std::basic_string<TCHAR> strNormal, std::basic_string<TCHAR> strSpecular)
{
	MaterialData::MaterialInfo* pInfo = new MaterialData::MaterialInfo();

	pInfo->strName = strName;

	pInfo->diffuse = diffuse;
	pInfo->ambient = ambient;
	pInfo->specular = specular;

	pInfo->strDiffuseMap = strDiffuse;
	pInfo->strNormalMap = strNormal;
	pInfo->strSpecularMap = strSpecular;

	return pInfo;
}
