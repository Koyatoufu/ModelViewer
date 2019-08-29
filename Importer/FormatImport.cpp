#include "FormatImporter.h"

CFormatImporter::CFormatImporter()
{
	for (int i = 0; i < E_IMPORT_FORMAT_MAX; i++)
	{
		m_parModelLoadFuntions[i] = nullptr;
		m_parModelImpoter[i] = nullptr;
	}

	m_parModelLoadFuntions[E_IMPORT_FORMAT_TXT] = &CFormatImporter::TxtLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_OBJ] = &CFormatImporter::ObjLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_FBX] = &CFormatImporter::FbxLoad;
	m_parModelLoadFuntions[E_IMPORT_FORMAT_MD5] = &CFormatImporter::MD5Load;

}

CFormatImporter::~CFormatImporter()
{
}

ModelData * CFormatImporter::LoadModel(std::basic_string<TCHAR> strFileName, E_IMPORT_FORMAT_TYPE eType)
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

ModelData * CFormatImporter::TxtLoad(std::basic_string<TCHAR> strFileName)
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

	MeshGroup* pGroup = new MeshGroup();

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

		Vertex vertex;

		//pGroup->vtPositions.push_back(position);
		//pGroup->vtUVs.push_back(uv);
		//pGroup->vtNormals.push_back(normal);

		//pGroup->vtIndexDatas.push_back(i);
		//pGroup->vtNormalIndexDatas.push_back(i);
		//pGroup->vtUVIndexDatas.push_back(i);
		pGroup->vtVertices.push_back(vertex);
		pGroup->vtIndicies.push_back(i);
	}

	pModelData->vtMeshes.push_back(pGroup);

	// Close the model file.
	fIn.close();

	return pModelData;
}

ModelData * CFormatImporter::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}

//directX øﬁº’¡¬«•∞Ë obj ø¿∏•º’ ¡¬«•∞Ë
//blender or max∏∏ ¥Î¿¿?
ModelData * CFormatImporter::ObjLoad(std::basic_string<TCHAR> strFileName)
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

	MeshGroup* pGroup = nullptr;

	int nTotalVertex = 0;

	struct tempVertexGroup
	{
		std::vector<DirectX::XMFLOAT3> vtPositions;
		std::vector<DirectX::XMFLOAT2> vtUVs;
		std::vector<DirectX::XMFLOAT3> vtNormals;

		std::vector<unsigned int> vtIndexDatas;
		std::vector<unsigned int> vtUVIndexDatas;
		std::vector<unsigned int> vtNormalIndexDatas;
	};

	std::vector<tempVertexGroup*> vtVertexGroups;

	int nRes = 0;

	std::basic_string<TCHAR> strPath = GetPathName(strFileName);

	int nTempMinus = 0;
	int nTempUVMinus = 0;
	int nTempNormalMinus = 0;

	while (nRes != EOF)
	{
		TCHAR szLineHeader[256] = {};

		nRes = _ftscanf_s(pFIle, _T("%s"), szLineHeader,256);

		if (nRes == EOF)
			break;
		
		if (_tcscmp(szLineHeader, _T("mtllib")) == 0)
		{
			nRes = _ftscanf_s(pFIle, _T("%s"), szMtlName,256);
		}
		else if (_tcscmp(szLineHeader, _T("object")) == 0 || _tcscmp(szLineHeader, _T("o")) == 0)
		{
			nTotalVertex = 0;
			
			pGroup = new MeshGroup();
			vtVertexGroups.push_back(pGroup);

			TCHAR szGroupName[128] = {};

			nRes = _ftscanf_s(pFIle, _T("%s"), szGroupName, 128);

			pGroup->strGroupName = szGroupName;

			if (vtVertexGroups.size() > 1)
			{
				nTempMinus = 0;
				nTempUVMinus = 0;
				nTempNormalMinus = 0;

				for (size_t i = 0; i < vtVertexGroups.size(); ++i)
				{
					for (size_t j = 0; j < vtVertexGroups[i]->vtNormalIndexDatas.size(); ++j)
					{
						if (nTempMinus < (int)(vtVertexGroups[i]->vtIndexDatas[j]))
							nTempMinus = (int)(vtVertexGroups[i]->vtIndexDatas[j]);

						if (nTempUVMinus < (int)(vtVertexGroups[i]->vtUVIndexDatas[j]))
							nTempUVMinus = (int)(vtVertexGroups[i]->vtUVIndexDatas[j]);

						if (nTempNormalMinus < (int)(vtVertexGroups[i]->vtNormalIndexDatas[j]))
							nTempNormalMinus = (int)(vtVertexGroups[i]->vtNormalIndexDatas[j]);
					}				
				}

				nTempMinus += 1;
				nTempUVMinus += 1;
				nTempNormalMinus += 1;
			}
		}
		else if (_tcscmp(szLineHeader, _T("v")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 position;
				nRes = _ftscanf_s(pFIle, _T("%f %f %f\n"), &position.x, &position.y, &position.z );

				//position.z *= -1.0f;

				pGroup->vtPositions.push_back(position);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vt")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT2 UV;
				nRes = _ftscanf_s(pFIle, _T("%f %f\n"), &UV.x, &UV.y);

				UV.y = 1.0f - UV.y;		
				pGroup->vtUVs.push_back(UV);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vn")) == 0)
		{
			if (pGroup)
			{
				DirectX::XMFLOAT3 normal;
				nRes = _ftscanf_s(pFIle, _T("%f %f %f\n"), &normal.x, &normal.y, &normal.z);

				//normal.z *= -1.0f;	 				

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

				nRes = nMatches;

				int nGroupSize = (int)(vtVertexGroups.size());

				for (int i = 0; i < 3; i++)
				{
					bool isVertAlreadyExist = false;

					int nTempIndex = unArrVertexIndex[i] - 1;
					int nTempUVIndex = unArrUVIndex[i] - 1;
					int nTempNormalIndex = unArrNormalIndex[i] - 1;

					nTempIndex -= nTempMinus;
					nTempUVIndex -= nTempUVMinus;
					nTempNormalIndex -= nTempNormalMinus;

					if (nTotalVertex >= 3)
					{
						for (int nCheck = 0; nCheck < nTotalVertex; ++nCheck)
						{
							if (nTempIndex == pGroup->vtIndexDatas[nCheck] && isVertAlreadyExist == false )
							{
								if (nTempUVIndex == pGroup->vtUVIndexDatas[nCheck])
								{
									pGroup->vtIndicies.push_back(nCheck);
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
						pGroup->vtIndicies.push_back(nTotalVertex);
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

				nRes = _ftscanf_s(pFIle, _T("%s"), szMtlName,128);
				pGroup->strMtlName = szMtlName;
			}
		}
	}

	fclose(pFIle);

	if (vtVertexGroups.size() < 1)
	{
		SAFE_DELETE(pModelData);
	}
	else
	{
		for (size_t i = 0; i < vtVertexGroups.size(); ++i)
		{
			if (vtVertexGroups[i] == nullptr)
				continue;

			pModelData->vtMeshes.push_back(vtVertexGroups[i]);
		}

		if (_tcslen(szMtlName) > 0)
		{
			MaterialData* pMtlData = nullptr;

			pMtlData = LoadMaterialObj(strPath + szMtlName, strPath);

			if(pMtlData)
				pModelData->pMaterialData = pMtlData;
		}
	}

	return pModelData;
}

ModelData * CFormatImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	if (strFileName.empty())
		return nullptr;

	FILE* pFile = nullptr;

	errno_t error = _tfopen_s(&pFile, strFileName.c_str(), _T("r"));

	if (error)
		return nullptr;

	ModelData* pModelData = new ModelData();

	int nRes = 0;

	int nNumOfSubsets = 0;
	int nNumOfJoints = 0;

	std::vector<std::basic_string<TCHAR>> vtDiifuseNames;

	while (nRes != EOF)
	{
		TCHAR szLineHeader[256];

		nRes = _ftscanf_s(pFile, _T("%s"), szLineHeader, 256);
		
		if (nRes == EOF)
			break;

		if (_tcscmp(szLineHeader, _T("numJoints")) == 0)
		{
			size_t nSize = 0;
			
			nRes = _ftscanf_s(pFile, _T("%d"), &nNumOfJoints);

			if( nSize > 0 )
				pModelData->vtJoints.reserve(nNumOfJoints);
		}
		else if (_tcscmp(szLineHeader, _T("numMeshes")) == 0)
		{
			size_t nSize = 0;

			nRes = _ftscanf_s(pFile, _T("%d"), &nNumOfSubsets);

			if (nSize > 0)
				pModelData->vtMeshes.reserve(nNumOfSubsets);
		}
		else if (_tcscmp(szLineHeader, _T("joints")) == 0)
		{
			for (int i = 0; i < nNumOfJoints; ++i)
			{
				Joint joint;

				while (_tcsstr(szLineHeader, _T("\"")) != 0)
				{
					nRes = _ftscanf_s(pFile, _T("%s"), szLineHeader, 256);
				}
				
				joint.strName = szLineHeader;

				size_t nFind = joint.strName.find(_T("\""));

				while (nFind != joint.strName.npos)
				{
					joint.strName.erase(nFind);
				}

				nRes = _ftscanf_s(pFile, _T("\t%d ( %f %f %f ) ( %f %f %f )"), &joint.nParentID,
				//nRes = _ftscanf_s(pFile, _T(" %d ( %f %f %f ) ( %f %f %f )"), &joint.nParentID,
					&joint.position.x, &joint.position.y, &joint.position.z, 
					&joint.orientation.x, &joint.orientation.y, &joint.orientation.z);

				float t = 1.0f - (joint.orientation.x * joint.orientation.x)
					- (joint.orientation.y * joint.orientation.y)
					- (joint.orientation.z * joint.orientation.z);

				if (t < 0.0f)
					joint.orientation.w = 0.0f;
				else
					joint.orientation.w = -sqrtf(t);

				pModelData->vtJoints.push_back(joint);
			}

			if (pModelData->vtJoints.size() != nNumOfJoints)
			{
				SAFE_DELETE(pModelData);
				break;
			}
		}
		else if (_tcscmp(szLineHeader, _T("mesh")) == 0)
		{
			MeshGroup* pGroup = new MeshGroup();

			while (_tcscmp(szLineHeader, _T("}")) != 0)
			{
				nRes = _ftscanf_s(pFile, _T("%s"), szLineHeader, 256);

				if (_tcscmp(szLineHeader, _T("meshes:")) == 0)
				{
					TCHAR strName[128] = {};
					nRes = _ftscanf_s(pFile, _T("%s"), strName, 128);

					pGroup->strGroupName = strName;

					pGroup->strMtlName = _T("mtl_");
					pGroup->strMtlName += strName;
				}
				else if (_tcscmp(szLineHeader, _T("shader")) == 0)
				{
					while (_tcsstr(szLineHeader, _T("\"")) != 0)
					{
						nRes = _ftscanf_s(pFile, _T("%s"), szLineHeader, 256);
					}

					std::basic_string<TCHAR> strTemp = szLineHeader;

					size_t nFind = strTemp.find(_T("\""));

					while (nFind != strTemp.npos)
						strTemp.erase(nFind);

					vtDiifuseNames.push_back(strTemp);
				}
				else if (_tcscmp(szLineHeader, _T("numverts")) == 0)
				{
					int nVertexNum = 0;

					nRes = _ftscanf_s(pFile, _T(" %d"), &nVertexNum);

					if (nVertexNum < 1)
					{
						SAFE_DELETE(pGroup);
						continue;
					}

					for (int i = 0; i < nVertexNum; ++i)
					{
						while (_tcsstr(szLineHeader, _T("vert")) != 0)
						{
							nRes = _ftscanf_s(pFile, _T("%s"), szLineHeader, 256);
						}

						int nIndex = 0;

						DirectX::XMFLOAT2 uvTemp;
						int nStartWeight = 0;
						int nWeightCount = 0;

						nRes = _ftscanf_s(pFile, _T(" %d ( %f %f ) %d %d"), &nIndex,
							&uvTemp.x, &uvTemp.y, &nStartWeight, &nWeightCount);

						pGroup->vtUVs.push_back(uvTemp);
						pGroup->vtStartWeights.push_back(nStartWeight);
						pGroup->vtWeightCount.push_back(nWeightCount);
					}
				}
				else if (_tcscmp(szLineHeader, _T("numtris")) == 0)
				{

				}
				else if (_tcscmp(szLineHeader, _T("numweights")) == 0)
				{

				}
			}

			pModelData->vtMeshes.push_back(pGroup);
		}
	}

	if (pModelData->vtMeshes.size() != nNumOfSubsets)
	{
		SAFE_DELETE(pModelData);
	}
	else
	{
		MaterialData* pMaterialData = new MaterialData();

		for (size_t i = 0; i < pModelData->vtMeshes.size(); ++i)
		{
			MeshGroup* pGroup = pModelData->vtMeshes[i];

			if (pGroup == nullptr)
				continue;


		}
	}

	return pModelData;
}

MaterialData * CFormatImporter::LoadMaterialObj(std::basic_string<TCHAR> strFileName, std::basic_string<TCHAR> strPath)
{
	FILE* pFIle = nullptr;

	errno_t error = _tfopen_s(&pFIle, strFileName.c_str(), _T("r"));

	if (error)
	{
		std::basic_string<TCHAR> strTemp = strPath + strFileName;

		error = _tfopen_s(&pFIle, strTemp.c_str(), _T("r"));

		if (error)
			return nullptr;
	}

	int nRes = 0;

	MaterialData* pData = new MaterialData;
	std::vector<MaterialData::MaterialInfo*> vtInfos;
	MaterialData::MaterialInfo* pInfo = nullptr;

	while (nRes != EOF)
	{
		TCHAR szLineHeader[256] = {};

		nRes = _ftscanf_s(pFIle, _T("%s"), szLineHeader, 256);

		if (nRes == EOF)
			break;

		if (_tcscmp(szLineHeader, _T("newmtl")) == 0)
		{
			pInfo = new MaterialData::MaterialInfo();

			vtInfos.push_back(pInfo);

			TCHAR szMtlName[128];

			nRes = _ftscanf_s(pFIle, _T("%s"), szMtlName, 128);

			pInfo->strName = szMtlName;
		}
		else if (_tcscmp(szLineHeader, _T("Ka")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f %f %f"), &pInfo->ambient.x, &pInfo->ambient.y, &pInfo->ambient.z);
		}
		else if (_tcscmp(szLineHeader, _T("Kd")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f %f %f"), &pInfo->diffuse.x, &pInfo->diffuse.y, &pInfo->diffuse.z);
		}
		else if (_tcscmp(szLineHeader, _T("Ks")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f %f %f"), &pInfo->specular.x, &pInfo->specular.y, &pInfo->specular.z);
		}
		else if (_tcscmp(szLineHeader, _T("ns")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f"), &pInfo->fSpecularDistance);
		}
		else if (_tcscmp(szLineHeader, _T("d")) == 0 || _tcscmp(szLineHeader, _T("Tr")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f"), &pInfo->fSpecularDistance);
		}
		else if (_tcscmp(szLineHeader, _T("illum")) == 0)
		{
			if (pInfo)
				nRes = _ftscanf_s(pFIle, _T("%f"), &pInfo->fSpecularDistance);
		}
		else if (_tcscmp(szLineHeader, _T("map_Kd")) == 0)
		{
			TCHAR szMapName[128];

			if (pInfo)
			{
				nRes = _ftscanf_s(pFIle, _T("%s"), szMapName, 128);

				pInfo->strDiffuseMap = strPath + szMapName;
			}
		}
		else if (_tcscmp(szLineHeader, _T("map_Ks")) == 0)
		{
			TCHAR szMapName[128];

			if (pInfo)
			{
				nRes = _ftscanf_s(pFIle, _T("%s"), szMapName, 128);

				pInfo->strSpecularMap = strPath + szMapName;
			}
		}
		else if (_tcscmp(szLineHeader, _T("map_Ka")) == 0)
		{
			TCHAR szMapName[128];

			if (pInfo)
			{
				nRes = _ftscanf_s(pFIle, _T("%s"), szMapName, 128);

				pInfo->strAmbientMap = strPath + szMapName;
			}
		}
		else if (_tcscmp(szLineHeader, _T("map_d")) == 0)
		{
			TCHAR szMapName[128];

			if (pInfo)
			{
				nRes = _ftscanf_s(pFIle, _T("%s"), szMapName, 128);

				pInfo->strAlphaMap = strPath + szMapName;
			}
		}
		//else if (_tcscmp(szLineHeader, _T("Ke")) == 0)
		//{

		//}
		//else if (_tcscmp(szLineHeader, _T("Ni")) == 0)
		//{

		//}
	}

	fclose(pFIle);

	if (vtInfos.size() < 1)
	{
		SAFE_DELETE(pData);
	}
	else
	{
		for (int i = 0; i < vtInfos.size(); ++i)
		{
			MaterialData::MaterialInfo* pInfo = vtInfos[i];

			if (pInfo == nullptr)
				continue;

			pData->vtMaterialInfo.push_back(pInfo);
		}
	}

	return pData;
}

std::basic_string<TCHAR> CFormatImporter::GetPathName(std::basic_string<TCHAR> strFileName)
{
	std::basic_string<TCHAR> strPath = strFileName;

	size_t nFound = strPath.find_last_of(_T("\\"));

	if (nFound == 0)
		return _T("");

	strPath = strPath.substr(0,nFound + 1);

	return strPath;
}
