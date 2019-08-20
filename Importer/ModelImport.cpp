#include "ModelImport.h"

void ImporterUtil::Model::SetFunctionPointSet()
{
	pModelLoadFuntions[E_IMPORT_FORMAT_TXT] = TxtLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = ObjLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_FBX] = FbxLoad;
	pModelLoadFuntions[E_IMPORT_FORMAT_MD5] = MD5Load;
}

ModelData * ImporterUtil::Model::TxtLoad(std::basic_string<TCHAR> strFileName)
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

	// Close the model file.
	fIn.close();

	pModelData->parVertices = pArData;

	return pModelData;
}

ModelData * ImporterUtil::Model::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImporterUtil::Model::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}

ModelData * ImporterUtil::Model::MD5Load(std::basic_string<TCHAR> strFileName)
{
	return nullptr;
}
