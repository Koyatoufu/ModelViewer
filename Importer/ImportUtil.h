#pragma once
#include "ImportDefine.h"

namespace ImportUtil
{
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);

	ModelData* LoadModelData(std::basic_string<TCHAR> strFileName);

}