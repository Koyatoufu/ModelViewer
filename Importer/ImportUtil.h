#pragma once
#include "ImportDefine.h"

namespace ImporterUtil
{
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);

	ModelData* LoadModelData(std::basic_string<TCHAR> strFileName);
}