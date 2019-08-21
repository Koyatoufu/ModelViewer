#pragma once

//#pragma comment(lib,"d3d11.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib,"D3DCompiler.lib")

#include <DirectXTex.h>

#include <iostream>
#include <fstream>

#include <string>
#include <tchar.h>

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
	int nMaterialIdx;

	VertexData() :position(0.0f,0.0f,0.0f), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f),nMaterialIdx(0) {};
};

struct MaterialData
{
	struct MaterialInfo
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;

		std::basic_string<TCHAR> strNormalMap;
		std::basic_string<TCHAR> strDiffuseMap;
		std::basic_string<TCHAR> strSpecularMap;
	};

	int nMaterialCount;
	MaterialInfo* parMaterialInfo;
};

struct ModelData
{
	int nVertexCount;
	int nIndexCount;
	VertexData *parVertices;
	bool bSkinned;
	MaterialData *pMateiralData;

	ModelData() : nVertexCount(0),nIndexCount(0),parVertices(NULL),bSkinned(false), pMateiralData(NULL){}
	~ModelData() 
	{
		if (pMateiralData)
		{
			delete pMateiralData;
			pMateiralData = nullptr;
		}

		if (parVertices)
		{
			delete[] parVertices;
			parVertices = nullptr;
		}
	}
};