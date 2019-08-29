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

struct Joint
{
	std::basic_string<TCHAR> strName;
	int nParentID;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 orientation;

	Joint() : nParentID(0), position(0.0f, 0.0f, 0.0f), orientation(0.0f, 0.0f, 0.0f, 1.0f) {}
};

struct Weight
{
	int nJointID;
	float fBias;
	DirectX::XMFLOAT3 position;

	Weight() : nJointID(0), fBias(0.0f), position(0.0f, 0.0f, 0.0f) {}
};

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;

	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 biTangent;

	int nStartWeight;
	int nWeightCount;

	Vertex() :
		position(0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f),
		tangent(0.0f, 0.0f, 0.0f), biTangent(0.0f, 0.0f, 0.0f),
		nStartWeight(0), nWeightCount(0) {}
};

struct MeshGroup
{
	std::vector<Vertex> vtVertices;
	std::vector<DWORD> vtIndicies;

	std::vector<Weight> vtWeights;

	//std::vector<DirectX::XMFLOAT3> vtPositions;
	//std::vector<DirectX::XMFLOAT2> vtUVs;
	//std::vector<DirectX::XMFLOAT3> vtNormals;

	//std::vector<DWORD> vtStartWeights;
	//std::vector<DWORD> vtWeightCount;

	//std::vector<unsigned int> vtIndexDatas;
	//std::vector<unsigned int> vtUVIndexDatas;
	//std::vector<unsigned int> vtNormalIndexDatas;

	std::basic_string<TCHAR> strGroupName;
	std::basic_string<TCHAR> strMtlName;

	int nMaterialIdx;

	MeshGroup():nMaterialIdx(0){}
	~MeshGroup() {}
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

		std::basic_string<TCHAR> strAmbientMap;
		std::basic_string<TCHAR> strAlphaMap;

		float fSpecularDistance;
		float fAlpha;
		int nIluminationOption;
	};

	std::vector<MaterialInfo*> vtMaterialInfo;

	~MaterialData()
	{
		for (size_t i = 0; i < vtMaterialInfo.size(); ++i)
		{
			SAFE_DELETE(vtMaterialInfo[i]);
		}
	}
};

struct ModelData
{
	std::vector<Joint> vtJoints;

	std::vector< MeshGroup* > vtMeshes;
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