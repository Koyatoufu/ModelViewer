#pragma once
//////////////
// INCLUDES
#include "RenderDefine.h"
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class CColorModel
{
private:
	struct  ColorVertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	CColorModel();
	CColorModel(const CColorModel&);
	~CColorModel();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};