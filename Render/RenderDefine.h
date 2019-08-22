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

//struct VertexType
//{
//	DirectX::XMFLOAT3 position;
//	DirectX::XMFLOAT2 UV;
//	DirectX::XMFLOAT3 normal;
//};

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

