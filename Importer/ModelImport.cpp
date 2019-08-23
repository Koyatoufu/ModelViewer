#include "ModelImporter.h"
#include "MaterialImport.h"

CModelImporter::CModelImporter()
{
	for (int i = 0; i < E_IMPORT_FORMAT_MAX; i++)
	{
		m_parModelLoadFuntions[i] = nullptr;
		m_parModelImpoter[i] = nullptr;
	}

	m_parModelLoadFuntions[E_IMPORT_FORMAT_TXT] = &CModelImporter::TxtLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = &CModelImporter::ObjLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_FBX] = &CModelImporter::FbxLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_MD5] = &CModelImporter::MD5Load;

}

CModelImporter::~CModelImporter()
{
}

ModelData * CModelImporter::LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType)
{
	if (strFileName.empty())
		return nullptr;

	if (eType == E_IMPORT_FORMAT_MAX)
		return nullptr;

	ModelData* pData = nullptr;

	if (m_parModelLoadFuntions[eType])
	{
		pData = (this->*m_parModelLoadFuntions[eType])(strFileName);
	}

	return pData;
}

ModelData * CModelImporter::TxtLoad(std::basic_string<TCHAR> strFileName)
{
	std::ifstream fIn;

	fIn.open(strFileName.c_str());

	if (fIn.fail())
		return nullptr;

	// Read up to the value of vertex count.
	char input;
	fIn.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}

	ModelData* pModelData = new ModelData();

	VertexGroup* pGroup = new VertexGroup();

	int nVertexCount = 0;

	// Read in the vertex count.
	fIn >> nVertexCount;

	// Set the number of indices to be the same as the vertex count.
	// Create the model using the vertex count that was read in.
	

	// Read up to the beginning of the data.
	fIn.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}
	fIn.get(input);
	fIn.get(input);

	// Read in the vertex data.
	for (int i = 0; i < nVertexCount; ++i)
	{
		VertexData* pVertexData = new VertexData();

		fIn >> pVertexData->position.x >> pVertexData->position.y >> pVertexData->position.z;
		fIn >> pVertexData->uv.x >> pVertexData->uv.y;
		fIn >> pVertexData->normal.x >> pVertexData->normal.y >> pVertexData->normal.z;

		pGroup->vtVertexDatas.push_back(pVertexData);
		pGroup->vtIndexDatas.push_back(i);
	}

	pModelData->vtMeshes.push_back(pGroup);

	// Close the model file.
	fIn.close();

	return pModelData;
}

ModelData * CModelImporter::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}

ModelData * CModelImporter::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}

ModelData * CModelImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}