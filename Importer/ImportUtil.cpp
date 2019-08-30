#include "ImportUtil.h"
#include "FormatImporter.h"

CImportUtil::CImportUtil()
{
	m_pModelImporter = new CFormatImporter();
}
CImportUtil::~CImportUtil()
{
	SAFE_DELETE(m_pModelImporter);
}

DirectX::ScratchImage CImportUtil::LoadTextureFromFile(std::basic_string<TCHAR> strFileName)
{
	DirectX::ScratchImage image;

	TCHAR szExt[256];
	_tsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);

	if (_tcsicmp(szExt, _T(".dds")) == 0)
	{
		DirectX::LoadFromDDSFile(strFileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_tcsicmp(szExt, _T(".tga")) == 0)
	{
		DirectX::LoadFromTGAFile(strFileName.c_str(), nullptr, image);
	}
	else if (_tcsicmp(szExt, _T(".wic")) == 0)
	{
		DirectX::LoadFromWICFile(strFileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	}

	return image;
}

ModelData* CImportUtil::LoadModelData(std::basic_string<TCHAR> strFileName)
{
	TCHAR szExt[256];
	_tsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);

	E_IMPORT_FORMAT_TYPE eImportFormat = E_IMPORT_FORMAT_MAX;

	if (_tcsicmp(szExt, _T(".txt")) == 0)
		eImportFormat = E_IMPORT_FORMAT_TXT;
	else if (_tcsicmp(szExt, _T(".obj")) == 0)
		eImportFormat = E_IMPORT_FORMAT_OBJ;
	else if (_tcsicmp(szExt, _T(".fbx")) == 0)
		eImportFormat = E_IMPORT_FORMAT_FBX;
	else if (_tcsicmp(szExt, _T(".md5mesh")) == 0)
		eImportFormat = E_IMPORT_FORMAT_MD5;

	if(eImportFormat == E_IMPORT_FORMAT_MAX)
		return nullptr;

	ModelData* pModelData = nullptr;

	if (m_pModelImporter)
	{
		pModelData = m_pModelImporter->LoadModel(strFileName, eImportFormat);
	}

	return pModelData;
}

void * CImportUtil::LoadAnimData(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}
