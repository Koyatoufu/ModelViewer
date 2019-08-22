#pragma once
#include "ImportDefine.h"

namespace ImportUtil
{
	MaterialData* LoadMtlFormat(std::basic_string<TCHAR> strFileName);

	MaterialData* CreateMaterialData(int nCount, std::vector<MaterialData::MaterialInfo*> vtMaterialInfo);

	MaterialData::MaterialInfo* CreateMaterialInfo( std::basic_string<TCHAR> strName,
		DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 specular,
		std::basic_string<TCHAR> strDiffuse, std::basic_string<TCHAR> strNormal, std::basic_string<TCHAR> strSpecular );
}