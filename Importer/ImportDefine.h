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

struct VertexData
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;

	VertexData() :position(0.0f,0.0f,0.0f), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {};
};

struct VertexGroup
{
	std::vector<VertexData*> vtVertexDatas;
	std::vector<int> vtIndexDatas;

	int nMaterialIdx;
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
		SAFE_DELETE(pMaterialData);
	}
};