#pragma once
#include "BaseModel.h"

class CLightModel :public CBaseModel 
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 normal;
	};


};