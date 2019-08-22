#include "ModelImport.h"
#include "MaterialImport.h"

CModelImporter::CModelImporter()
{
}

CModelImporter::~CModelImporter()
{
}

void CModelImporter::SetFunctionPointSet()
{
	//CModelImporter::pModelLoadFuntions[E_IMPORT_FORMAT_TXT] = CModelImporter::TxtLoad;
	//CModelImporter::pModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = CModelImporter::ObjLoad;
	//CModelImporter::pModelLoadFuntions[E_IMPORT_FORMAT_FBX] = CModelImporter::FbxLoad;
	//CModelImporter::pModelLoadFuntions[E_IMPORT_FORMAT_MD5] = CModelImporter::MD5Load;
}

ModelData * CModelImporter::LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType)
{
	return nullptr;
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

	// Close the model file.
	fIn.close();

	return pModelData;
}

ModelData * CModelImporter::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * CModelImporter::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	FILE* pFile = nullptr;

	errno_t error = _tfopen_s(&pFile, strFileName.c_str(), _T("r"));

	if (error != 0)
	{
		return nullptr;
	}

	ModelData* pModelData = new ModelData();

	char szLine[128] = "";

	while (feof(pFile))
	{
		fscanf_s(pFile, "%s", szLine);

		if (strstr(szLine, "mtllib"))
		{

		}
	}

	return pModelData;
}

ModelData * CModelImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}