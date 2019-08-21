#include "ModelImport.h"
#include "MaterialImport.h"

void ImportUtil::Model::SetFunctionPointSet()
{
	pModelLoadFuntions[E_IMPORT_FORMAT_TXT] = TxtLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = ObjLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_FBX] = FbxLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_MD5] = MD5Load;
}

ModelData * ImportUtil::Model::TxtLoad(std::basic_string<TCHAR> strFileName)
{
	std::ifstream fIn;
	
	fIn.open(strFileName.c_str());

	if(fIn.fail())
		return nullptr;

	// Read up to the value of vertex count.
	char input;
	fIn.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}

	ModelData* pModelData = new ModelData();

	// Read in the vertex count.
	fIn >> pModelData->nVertexCount;

	// Set the number of indices to be the same as the vertex count.
	pModelData->nIndexCount = pModelData->nVertexCount;

	// Create the model using the vertex count that was read in.
	VertexData* pArData = new VertexData[pModelData->nVertexCount];

	// Read up to the beginning of the data.
	fIn.get(input);
	while (input != ':')
	{
		fIn.get(input);
	}
	fIn.get(input);
	fIn.get(input);

	// Read in the vertex data.
	for (int i = 0; i < pModelData->nVertexCount; ++i)
	{
		fIn >> pArData[i].position.x >> pArData[i].position.y >> pArData[i].position.z;
		fIn >> pArData[i].uv.x >> pArData[i].uv.y;
		fIn >> pArData[i].normal.x >> pArData[i].normal.y >> pArData[i].normal.z;
	}
	
	pModelData->parVertices = pArData;

	// Close the model file.
	fIn.close();

	return pModelData;
}

ModelData * ImportUtil::Model::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImportUtil::Model::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	FILE* pFile = nullptr;
	
	errno_t error = _tfopen_s( &pFile,strFileName.c_str(), _T("r"));

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

ModelData * ImportUtil::Model::MD5Load(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}
