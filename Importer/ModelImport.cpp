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

	unsigned int nVertexCount = 0;

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
	for ( unsigned int i = 0; i < nVertexCount; ++i)
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;

		fIn >> position.x >> position.y >> position.z;
		fIn >> uv.x >> uv.y;
		fIn >> normal.x >> normal.y >> normal.z;

		pGroup->vtPositions.push_back(position);
		pGroup->vtUVs.push_back(uv);
		pGroup->vtNormals.push_back(normal);

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
	if (strFileName.empty())
		return nullptr;

	ModelData* pModelData = nullptr;

	FILE* pFIle = nullptr;

	errno_t error = _tfopen_s(&pFIle,strFileName.c_str(),_T("r"));

	if (error)
		return nullptr;
	
	int nRes = 1;

	TCHAR szMtlName[128] = {};

	pModelData = new ModelData();

	VertexGroup* pGroup = nullptr;

	while (nRes != EOF)
	{
		TCHAR szLineHeader[128] = {};

		nRes = _ftscanf_s(pFIle, _T("%s"), szLineHeader);

		if (nRes == EOF)
		{
			if(pGroup)
				pModelData->vtMeshes.push_back(pGroup);
			break;
		}

		if (_tcscmp(szLineHeader, _T("mtllib")))
		{
			_ftscanf_s(pFIle, _T("%s"), szMtlName);

			MessageBox(NULL, szMtlName, _T("Material Load Test"), MB_OK);
		}
		//else if (_tcscmp(szLineHeader, _T("o")))
		//{
		//	if (pGroup == nullptr)
		//	{
		//		pGroup = new VertexGroup();
		//	}
		//	else
		//	{
		//		pModelData->vtMeshes.push_back(pGroup);
		//		pGroup = new VertexGroup();
		//	}

		//	TCHAR szGroupName[128] = {};

		//	_ftscanf_s(pFIle, _T("%s"),szGroupName);

		//	pGroup->strGroupName = szGroupName;
		//}
		else if (_tcscmp(szLineHeader, _T("v")))
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 position;
				_ftscanf_s(pFIle, _T("%f %f %f\n"), &position.x, &position.y, &position.z );
				pGroup->vtPositions.push_back(position);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vt")))
		{
			if (pGroup)
			{
				DirectX::XMFLOAT2 UV;
				_ftscanf_s(pFIle, _T("%f %f\n"), &UV.x, &UV.y);
				pGroup->vtUVs.push_back(UV);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vn")))
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 normal;
				_ftscanf_s(pFIle, _T("%f %f %f\n"), &normal.x, &normal.y, &normal.z);
				pGroup->vtPositions.push_back(normal);
			}
		}
		else if (_tcscmp(szLineHeader, _T("f")))
		{
			if (pGroup)
			{
				unsigned int unArrVertexIndex[3] = {}, unArrUVIndex[3] = {}, unArrNormalIndex[3] = {};
				int nMatches = _ftscanf_s(pFIle, _T("%d/%d/%d %d/%d/%d %d/%d/%d\n"), 
					&unArrNormalIndex[0], &unArrUVIndex[0], &unArrNormalIndex[0]
					, &unArrNormalIndex[1], &unArrUVIndex[1], &unArrNormalIndex[1]
					, &unArrNormalIndex[2], &unArrUVIndex[2], &unArrNormalIndex[2]);

				if (nMatches != 9)
				{
					SAFE_DELETE(pGroup);
					break;
				}

				for (int i = 0; i < 3; i++)
				{
					pGroup->vtIndexDatas.push_back(unArrVertexIndex[i]);
					pGroup->vtUVIndexDatas.push_back(unArrUVIndex[i]);
					pGroup->vtNormalIndexDatas.push_back(unArrNormalIndex[i]);
				}
			}
		}
		else if (_tcscmp(szLineHeader, _T("usemtl")))
		{
			if (pGroup)
			{
				TCHAR szMtlName[128];

				_ftscanf_s(pFIle, _T("%s"), szMtlName);
				pGroup->strMtlName = szMtlName;
			}
		}
	}

	if (pGroup == nullptr)
	{
		SAFE_DELETE(pModelData);
	}

	return pModelData;
}

ModelData * CModelImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}