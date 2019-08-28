#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <tchar.h>

#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <list>

#define SAFE_DELETE(point) { if(point){ delete point; point = nullptr; } }
#define SAFE_DELETE_ARRAY(pointArray) { if(pointArray){ delete [] pointArray; pointArray = nullptr; } }
#define SAFE_RELEASE_D3DCONTENTS( pContents ) { if(pContents){ pContents->Release(); pContents = nullptr; } }

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct LightBufferType
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 specularColor;
	DirectX::XMFLOAT3 lightDirection;
	float specularPower;
};

struct CameraBufferType
{
	DirectX::XMFLOAT3 cameraPosition;
	float padding;
};

enum ETextureType
{
	ETEXTURE_DIFFUSE,
	ETEXTURE_NORMAL,
	ETEXTURE_SPECULAR,
	ETEXTURE_CUBEMAP,
	ETEXTURE_ALPHAMAP,
	ETEXTURE_REFLECTMAP,
	ETEXTURE_MAX
};

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;

	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 biTangent;

	int nStartWeight;
	int nWeightCount;

	VertexType(): 
		position(0.0f, 0.0f, 0.0f),UV(0.0f, 0.0f),normal(0.0f, 0.0f, 0.0f),
		tangent(0.0f, 0.0f, 0.0f),biTangent(0.0f, 0.0f, 0.0f), 
		nStartWeight(0),nWeightCount(0) {}
};

struct JointType
{
	std::basic_string<TCHAR> strName;
	int nParentID;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 orientation;

	JointType() : nParentID(0),position(0.0f,0.0f,0.0f),orientation(0.0f,0.0f,0.0f,1.0f){}
};

struct WeightType
{
	int nJointID;
	float fBias;
	DirectX::XMFLOAT3 position;

	WeightType() : nJointID(0),fBias(0.0f),position(0.0f,0.0f,0.0f){}
};

struct ModelSubsets
{
	int nNumTriangles;
	int nTexArrayIndex;

	std::vector<VertexType> vtVertices;
	std::vector<DWORD>		vtIndices;
	std::vector<WeightType>		vtWeights;
	
	std::basic_string<TCHAR> strMtlName;

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	ModelSubsets() : nNumTriangles(0), nTexArrayIndex(0), pVertexBuffer(nullptr), pIndexBuffer(nullptr) {}

	~ModelSubsets()
	{
		SAFE_RELEASE_D3DCONTENTS(pVertexBuffer);
		SAFE_RELEASE_D3DCONTENTS(pIndexBuffer);
	}
};