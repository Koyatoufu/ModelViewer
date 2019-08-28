#pragma once

//#pragma comment(lib,"d3d11.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"D3DCompiler.lib")

#include <DirectXTex.h>

#include <iostream>
#include <fstream>

#include <string>
#include <tchar.h>

#define SAFE_DELETE(point) { if(point){ delete point; point = nullptr; } }
#define SAFE_DELETE_ARRAY(pointArray) { if(pointArray){ delete [] pointArray; pointArray = nullptr; } }
#define SAFE_RELEASE_D3DCONTENTS( pContents ) { if(pContents){ pContents->Release(); pContents = nullptr; } }

enum E_IMPORT_FORMAT_TYPE
{
	E_IMPORT_FORMAT_TXT,
	E_IMPORT_FORMAT_OBJ,
	E_IMPORT_FORMAT_FBX,
	E_IMPORT_FORMAT_MD5,
	E_IMPORT_FORMAT_MAX
};

struct VertexGroup
{
	std::vector<DirectX::XMFLOAT3> vtPositions;
	std::vector<DirectX::XMFLOAT2> vtUVs;
	std::vector<DirectX::XMFLOAT3> vtNormals;

	std::vector<unsigned int> vtIndexDatas;
	std::vector<unsigned int> vtUVIndexDatas;
	std::vector<unsigned int> vtNormalIndexDatas;

	std::vector<DWORD> vtTotalIndicies;

	std::basic_string<TCHAR> strGroupName;
	std::basic_string<TCHAR> strMtlName;

	int nMaterialIdx;

	VertexGroup():nMaterialIdx(0) {}
	~VertexGroup() {}
};

struct MaterialData
{
	struct MaterialInfo
	{
		std::basic_string<TCHAR> strName;

		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;

		std::basic_string<TCHAR> strNormalMap;
		std::basic_string<TCHAR> strDiffuseMap;
		std::basic_string<TCHAR> strSpecularMap;
	};

	std::vector<MaterialInfo*> vtMaterialInfo;
};

struct ModelData
{
	std::vector< VertexGroup* > vtMeshes;
	MaterialData* pMaterialData;
	bool bSkinned;

	ModelData() :bSkinned(false), pMaterialData(nullptr){}
	~ModelData()
	{
		for (int i = 0; i < vtMeshes.size(); ++i)
		{
			SAFE_DELETE(vtMeshes[i]);
		}
		vtMeshes.clear();

		SAFE_DELETE(pMaterialData);
	}
};