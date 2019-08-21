#include "MaterialImport.h"

MaterialData * ImportUtil::LoadMtlFormat(std::basic_string<TCHAR> strFileName)
{
	if (strFileName.empty())
		return nullptr;

	std::ifstream fin;

	return nullptr;
}

MaterialData * ImportUtil::CreateMaterialData(int nCount, MaterialData::MaterialInfo * parMaterialInfo)
{
	if (nCount < 1)
		return nullptr;

	MaterialData* pData = new MaterialData();

	pData->nMaterialCount = nCount;
	pData->parMaterialInfo = parMaterialInfo;

	return pData;
}

MaterialData::MaterialInfo ImportUtil::CreateMaterialInfo(DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 specular,
	std::basic_string<TCHAR> strDiffuse, std::basic_string<TCHAR> strNormal, std::basic_string<TCHAR> strSpecular)
{
	MaterialData::MaterialInfo info;

	info.diffuse = diffuse;
	info.ambient = ambient;
	info.specular = specular;

	info.strDiffuseMap = strDiffuse;
	info.strNormalMap = strNormal;
	info.strSpecularMap = strSpecular;

	return info;
}
