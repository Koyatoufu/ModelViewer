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
		pGroup->vtNormalIndexDatas.push_back(i);
		pGroup->vtUVIndexDatas.push_back(i);

		pGroup->vtTotalIndicies.push_back(i);
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

//directX ¿Þ¼ÕÁÂÇ¥²¾ obj ¿À¸¥¼Õ ÁÂÇ¥°è
//blender or max¸¸ ´ëÀÀ?
ModelData * CModelImporter::ObjLoad(std::basic_string<TCHAR> strFileName)
{
	if (strFileName.empty())
		return nullptr;

	ModelData* pModelData = nullptr;

	FILE* pFIle = nullptr;

	errno_t error = _tfopen_s(&pFIle,strFileName.c_str(),_T("r"));

	if (error)
		return nullptr;
	
	TCHAR szMtlName[256] = {};

	pModelData = new ModelData();

	VertexGroup* pGroup = nullptr;

	int nTotalVertex = 0;

	while (true)
	{
		TCHAR szLineHeader[256] = {};

		int nRes = _ftscanf_s(pFIle, _T("%s"), szLineHeader,256);

		if (nRes == EOF)
			break;
		
		if (_tcscmp(szLineHeader, _T("mtllib")) == 0)
		{
			_ftscanf_s(pFIle, _T("%s"), szMtlName,256);
		}
		else if (_tcscmp(szLineHeader, _T("object")) == 0 || _tcscmp(szLineHeader, _T("o")) == 0)
		{
			if (pGroup)
			{
				pModelData->vtMeshes.push_back(pGroup);
				nTotalVertex = 0;
			}

			pGroup = new VertexGroup();

			TCHAR szGroupName[128] = {};

			_ftscanf_s(pFIle, _T("%s"), szGroupName, 128);

			pGroup->strGroupName = szGroupName;
		}
		else if (_tcscmp(szLineHeader, _T("v")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 position;
				_ftscanf_s(pFIle, _T("%f %f %f\n"), &position.x, &position.y, &position.z );

				position.z *= -1.0f;

				pGroup->vtPositions.push_back(position);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vt")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT2 UV;
				_ftscanf_s(pFIle, _T("%f %f\n"), &UV.x, &UV.y);

				UV.y = 1.0f - UV.y;		//directX ¿Þ¼ÕÁÂÇ¥²¾ obj ¿À¸¥¼Õ ÁÂÇ¥°è
				pGroup->vtUVs.push_back(UV);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vn")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 normal;
				_ftscanf_s(pFIle, _T("%f %f %f\n"), &normal.x, &normal.y, &normal.z);

				normal.z *= -1.0f;	 				//directX ¿Þ¼ÕÁÂÇ¥²¾ obj ¿À¸¥¼Õ ÁÂÇ¥°è

				pGroup->vtNormals.push_back(normal);
			}
		}
		else if (_tcscmp(szLineHeader, _T("f")) == 0)
		{
			if (pGroup)
			{
				unsigned int unArrVertexIndex[3] = {}, unArrUVIndex[3] = {}, unArrNormalIndex[3] = {};
				int nMatches = _ftscanf_s(pFIle, _T("%d/%d/%d %d/%d/%d %d/%d/%d\n"), 
					&unArrVertexIndex[0], &unArrUVIndex[0], &unArrNormalIndex[0]
					, &unArrVertexIndex[1], &unArrUVIndex[1], &unArrNormalIndex[1]
					, &unArrVertexIndex[2], &unArrUVIndex[2], &unArrNormalIndex[2]);

				if (nMatches != 9)
				{
					SAFE_DELETE(pGroup);
					break;
				}

				for (int i = 0; i < 3; i++)
				{
					bool isVertAlreadyExist = false;

					int nTempIndex = unArrVertexIndex[i] - 1 ;
					int nTempUVIndex = unArrUVIndex[i] - 1;
					int nTempNormalIndex = unArrNormalIndex[i] - 1;

					if (nTotalVertex >= 3)
					{
						for (int nCheck = 0; nCheck < nTotalVertex; ++nCheck)
						{
							if (nTempIndex == pGroup->vtIndexDatas[nCheck] && isVertAlreadyExist == false )
							{
								if (nTempUVIndex == pGroup->vtUVIndexDatas[nCheck])
								{
									pGroup->vtTotalIndicies.push_back(nCheck);
									isVertAlreadyExist = true;
								}
							}
						}
					}

					if (isVertAlreadyExist == false)
					{
						pGroup->vtIndexDatas.push_back(nTempIndex);
						pGroup->vtUVIndexDatas.push_back(nTempUVIndex);
						pGroup->vtNormalIndexDatas.push_back(nTempNormalIndex);
						pGroup->vtTotalIndicies.push_back(nTotalVertex);
						nTotalVertex++;
					}
				}


			}
		}
		else if (_tcscmp(szLineHeader, _T("usemtl")) == 0)
		{
			if (pGroup)
			{
				TCHAR szMtlName[128];

				_ftscanf_s(pFIle, _T("%s"), szMtlName,128);
				pGroup->strMtlName = szMtlName;
			}
		}
	}

	if (pGroup == nullptr)
	{
		SAFE_DELETE(pModelData);
	}
	else
	{
		pModelData->vtMeshes.push_back(pGroup);
	}

	if (_tcslen(szMtlName) > 0)
	{


		//MessageBox(NULL, szMtlName, _T("¸ÓÅ×¸®¾ó ÀÌ¸§ È®ÀÎ"), MB_OK);
	}

	return pModelData;
}

ModelData * CModelImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}