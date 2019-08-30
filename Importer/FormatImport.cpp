#include "FormatImporter.h"

#include <map>

struct TempVertexGroup
{
	std::vector<DirectX::XMFLOAT3> vtPositions;
	std::vector<DirectX::XMFLOAT2> vtUVs;
	std::vector<DirectX::XMFLOAT3> vtNormals;

	std::vector<unsigned int> vtIndexDatas;
	std::vector<unsigned int> vtUVIndexDatas;
	std::vector<unsigned int> vtNormalIndexDatas;

	std::basic_string<TCHAR> strGroupName;
	std::basic_string<TCHAR> strMtlName;

	std::vector<DWORD> vtStartWeights;
	std::vector<DWORD> vtWeightCount;

	std::vector<DWORD> vtIndicies;
	std::vector<Weight> vtWeight;

	int nNumTri;
};

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
//LoadModel

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

	MeshSubsets* pGroup = new MeshSubsets();

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

		Vertex vertexTemp;

		vertexTemp.position = position;
		vertexTemp.UV = uv;
		vertexTemp.normal = normal;

		pGroup->vtVertices.push_back(vertexTemp);
		pGroup->vtIndicies.push_back(i);
	}

	pModelData->vtMeshSubsets.push_back(pGroup);

	// Close the model file.
	fIn.close();

	return pModelData;
}
//LoadTxt

ModelData * CFormatImporter::FbxLoad(std::basic_string<TCHAR> strFileName)
{
	ModelData* pModelData = nullptr;

	return pModelData;
}
//LoadFbx

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

	int nTotalVertex = 0;

	TempVertexGroup *pTempVertexGroup = nullptr;

	std::vector<TempVertexGroup*> vtVertexGroups;

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
			
			pTempVertexGroup = new TempVertexGroup();
			vtVertexGroups.push_back(pTempVertexGroup);

			TCHAR szGroupName[128] = {};

			nRes = _ftscanf_s(pFIle, _T("%s"), szGroupName, 128);

			pTempVertexGroup->strGroupName = szGroupName;

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
			if (pTempVertexGroup)
			{
				DirectX::XMFLOAT3 position;
				nRes = _ftscanf_s(pFIle, _T("%f %f %f\n"), &position.x, &position.y, &position.z );

				//position.z *= -1.0f;

				pTempVertexGroup->vtPositions.push_back(position);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vt")) == 0)
		{
			if (pTempVertexGroup)
			{
				DirectX::XMFLOAT2 UV;
				nRes = _ftscanf_s(pFIle, _T("%f %f\n"), &UV.x, &UV.y);

				UV.y = 1.0f - UV.y;		
				pTempVertexGroup->vtUVs.push_back(UV);
			}
		}
		else if (_tcscmp(szLineHeader, _T("vn")) == 0)
		{
			if (pTempVertexGroup)
			{
				DirectX::XMFLOAT3 normal;
				nRes = _ftscanf_s(pFIle, _T("%f %f %f\n"), &normal.x, &normal.y, &normal.z);

				//normal.z *= -1.0f;	 				

				pTempVertexGroup->vtNormals.push_back(normal);
			}
		}
		else if (_tcscmp(szLineHeader, _T("f")) == 0)
		{
			if (pTempVertexGroup)
			{
				unsigned int unArrVertexIndex[3] = {}, unArrUVIndex[3] = {}, unArrNormalIndex[3] = {};
				int nMatches = _ftscanf_s(pFIle, _T("%d/%d/%d %d/%d/%d %d/%d/%d\n"), 
					&unArrVertexIndex[0], &unArrUVIndex[0], &unArrNormalIndex[0]
					, &unArrVertexIndex[1], &unArrUVIndex[1], &unArrNormalIndex[1]
					, &unArrVertexIndex[2], &unArrUVIndex[2], &unArrNormalIndex[2]);

				if (nMatches != 9)
				{
					SAFE_DELETE(pTempVertexGroup);
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
							if (nTempIndex == pTempVertexGroup->vtIndexDatas[nCheck] && isVertAlreadyExist == false )
							{
								if (nTempUVIndex == pTempVertexGroup->vtUVIndexDatas[nCheck])
								{
									pTempVertexGroup->vtIndicies.push_back(nCheck);
									isVertAlreadyExist = true;
								}
							}
						}
					}

					if (isVertAlreadyExist == false)
					{
						pTempVertexGroup->vtIndexDatas.push_back(nTempIndex);
						pTempVertexGroup->vtUVIndexDatas.push_back(nTempUVIndex);
						pTempVertexGroup->vtNormalIndexDatas.push_back(nTempNormalIndex);
						pTempVertexGroup->vtIndicies.push_back(nTotalVertex);
						nTotalVertex++;
					}
				}


			}
		}
		else if (_tcscmp(szLineHeader, _T("usemtl")) == 0)
		{
			if (pTempVertexGroup)
			{
				TCHAR szMtlName[128];

				nRes = _ftscanf_s(pFIle, _T("%s"), szMtlName,128);
				pTempVertexGroup->strMtlName = szMtlName;
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

			TempVertexGroup* pGroup = vtVertexGroups[i];

			MeshSubsets* pMeshSubsets = new MeshSubsets();

			pMeshSubsets->strSubsetName = pGroup->strGroupName;
			pMeshSubsets->strMtlName = pGroup->strMtlName;

			for (int j = 0; j < pGroup->vtIndexDatas.size(); ++j)
			{
				Vertex vertex;

				vertex.position = pGroup->vtPositions[pGroup->vtIndexDatas[j]];
				vertex.normal = pGroup->vtNormals[pGroup->vtNormalIndexDatas[j]];
				vertex.UV = pGroup->vtUVs[pGroup->vtUVIndexDatas[j]];

				pMeshSubsets->vtVertices.push_back(vertex);
			}

			for (int j = 0; j < pGroup->vtIndicies.size(); ++j)
			{
				pMeshSubsets->vtIndicies.push_back(pGroup->vtIndicies[j]);
			}

			pModelData->vtMeshSubsets.push_back(pMeshSubsets);
		}

		if (_tcslen(szMtlName) > 0)
		{
			MaterialData* pMtlData = nullptr;

			pMtlData = LoadMaterialObj(strPath + szMtlName, strPath);

			if(pMtlData)
				pModelData->pMaterialData = pMtlData;
		}

		for (size_t i = 0; i < vtVertexGroups.size(); ++i)
		{
			SAFE_DELETE(vtVertexGroups[i]);
		}
	}

	return pModelData;
}
//LoadObj

ModelData * CFormatImporter::MD5Load(std::basic_string<TCHAR> strFileName)
{
	if (strFileName.empty())
		return nullptr;

	std::basic_ifstream<TCHAR, std::char_traits<TCHAR>> fileIn(strFileName);
	
	//std::wifstream wif;

	ModelData* pModelData = new ModelData();

	int nNumOfSubsets = 0;
	int nNumOfJoints = 0;

	std::map<std::basic_string<TCHAR>, std::basic_string<TCHAR>> mapMaterials;

	std::vector<TempVertexGroup> vtTemp;

	std::basic_string<TCHAR> strCheck;

	std::basic_string<TCHAR> strFilePath = GetPathName(strFileName);

	if (fileIn)
	{
		while (fileIn)
		{
			fileIn >> strCheck;

			if (strCheck.compare(_T("numJoints")) == 0)
			{
				fileIn >> nNumOfJoints;

				pModelData->vtJoints.reserve(nNumOfJoints);
			}
			else if (strCheck.compare(_T("numMeshes")) == 0)
			{
				fileIn >> nNumOfSubsets;

				pModelData->vtMeshSubsets.reserve(nNumOfSubsets);
			}
			else if (strCheck.compare(_T("joints")) == 0)
			{
				fileIn >> strCheck; // Skip the "{"

				for (int i = 0; i < nNumOfJoints; ++i)
				{
					Joint tempJoint;

					fileIn >> tempJoint.strName;

					if (tempJoint.strName[tempJoint.strName.size() - 1] != '"')
					{
						TCHAR checkChar;
						bool bIsIointNameFound = false;
						while (!bIsIointNameFound)
						{
							checkChar = fileIn.get();

							if (checkChar == '"')
								bIsIointNameFound = true;

							tempJoint.strName += checkChar;
						}
					}

					size_t nFind = tempJoint.strName.find(_T("\""));

					while (nFind != tempJoint.strName.npos)
					{
						tempJoint.strName.erase(nFind, 1);
						nFind = tempJoint.strName.find(_T("\""));
					}

					fileIn >> tempJoint.nParentID;

					fileIn >> strCheck; // Skip the "("

					fileIn >> tempJoint.position.x >> tempJoint.position.z >> tempJoint.position.y;

					fileIn >> strCheck >> strCheck; // Skip the ")" and "("

					fileIn >> tempJoint.orientation.x >> tempJoint.orientation.z >> tempJoint.orientation.y;

					float t = 1.0f - (tempJoint.orientation.x * tempJoint.orientation.x)
						- (tempJoint.orientation.y * tempJoint.orientation.y)
						- (tempJoint.orientation.z * tempJoint.orientation.z);

					if (t < 0.0f)
						tempJoint.orientation.w = 0.0f;
					else
						tempJoint.orientation.w = -sqrtf(t);

					pModelData->vtJoints.push_back(tempJoint);

					std::getline(fileIn, strCheck);		// Skip rest of this line
				}

				//if (pModelData->vtJoints.size() != nNumOfJoints)
				//	break;

				fileIn >> strCheck;					// Skip the "}"
			}
			else if (strCheck.compare(_T("mesh")) == 0)
			{
				fileIn >> strCheck; // Skip the "{"

				fileIn >> strCheck; // Skip the //

				fileIn >> strCheck;

				bool bFailed = false;

				TempVertexGroup tempVertexGroup;

				while ( strCheck.compare(_T("}")) != 0 )
				{
					if (strCheck.compare(_T("meshes:")) == 0)
					{
						fileIn >> strCheck;

						tempVertexGroup.strGroupName = strCheck;
					}
					else if (strCheck.compare(_T("shader")) == 0)
					{
						std::wstring strDiffusePath;
						fileIn >> strDiffusePath;			// Get texture's filename

						// Take spaces into account if filename or material name has a space in it
						if (strDiffusePath[strDiffusePath.size() - 1] != '"')
						{
							TCHAR checkChar;
							bool fileNameFound = false;
							while (!fileNameFound)
							{
								checkChar = fileIn.get();

								if (checkChar == '"')
									fileNameFound = true;

								strDiffusePath += checkChar;
							}
						}

						size_t nFind = strDiffusePath.find(_T("\""));

						while (nFind != strDiffusePath.npos)
						{
							strDiffusePath.erase(nFind, 1);
							nFind = strDiffusePath.find(_T("\""));
						}

						std::basic_string<TCHAR> strMtlName = strDiffusePath;

						nFind = strMtlName.find_last_of(_T("\\"));

						if (nFind != strMtlName.npos)
						{
							strMtlName.erase(0, nFind+1);
						}

						nFind = strMtlName.find_last_of(_T("."));

						if (nFind != strDiffusePath.npos)
						{
							strMtlName.erase(nFind, strDiffusePath.size() -1);
						}

						strMtlName = _T("mtl_") + strMtlName;
						tempVertexGroup.strMtlName = strMtlName;

						std::map<std::basic_string<TCHAR>, std::basic_string<TCHAR>>::iterator iter;
						
						iter = mapMaterials.find(strMtlName);

						if (iter == mapMaterials.end())
						{
							std::pair < std::basic_string<TCHAR>, std::basic_string<TCHAR >> pair;

							pair.first = strMtlName;
							pair.second = strFilePath + strDiffusePath;

							mapMaterials.insert(pair);
						}

						std::getline(fileIn, strCheck);				// Skip rest of this line
					}
					else if (strCheck.compare(_T("numverts")) == 0)
					{
						int nNumVertex = 0;

						fileIn >> nNumVertex;

						std::getline(fileIn, strCheck);				// Skip rest of this line

						for (int i = 0; i < nNumVertex; ++i)
						{
							int nIndex = 0;

							DirectX::XMFLOAT2 uvTemp;
							int nStartWeight = 0;
							int nWeightCount = 0;

							fileIn >> strCheck						// Skip "vert # ("
								>> nIndex							// Check Index
								>> strCheck;

							fileIn >> uvTemp.x >> uvTemp.y;

							fileIn >> strCheck;						// Skip ")"

							fileIn >> nStartWeight;
							fileIn >> nWeightCount;

							tempVertexGroup.vtUVs.push_back(uvTemp);
							tempVertexGroup.vtStartWeights.push_back(nStartWeight);
							tempVertexGroup.vtWeightCount.push_back(nWeightCount);

							std::getline(fileIn, strCheck);			// Skip rest of this line
						}
					}
					else if (strCheck.compare(_T("numtris")) == 0)
					{
						int nNumTri = 0;

						fileIn >> nNumTri;

						std::getline(fileIn, strCheck);				// Skip rest of this line

						tempVertexGroup.nNumTri = nNumTri;

						for (int i = 0; i < nNumTri; ++i)
						{
							fileIn >> strCheck; // Skip "tri"
							fileIn >> strCheck; // Skip tri counter

							int narTempIndex[3] = {};

							// Store the 3 indices

							fileIn >> narTempIndex[0];
							fileIn >> narTempIndex[1];
							fileIn >> narTempIndex[2];
							
							//fileIn >> narTempIndex[3];
							
							tempVertexGroup.vtIndicies.push_back(narTempIndex[0]);
							tempVertexGroup.vtIndicies.push_back(narTempIndex[1]);
							tempVertexGroup.vtIndicies.push_back(narTempIndex[2]);

							std::getline(fileIn, strCheck);				// Skip rest of this line
						}
					}
					else if (strCheck.compare(_T("numweights")) == 0)
					{
						int nNumWeights = 0;

						fileIn >> nNumWeights;

						std::getline(fileIn, strCheck);				// Skip rest of this line

						for (int i = 0; i < nNumWeights; ++i)
						{
							Weight tempWeight;

							fileIn >> strCheck >> strCheck;		// Skip "weight #"

							fileIn >> tempWeight.nJointID;				// Store weight's joint ID

							fileIn >> tempWeight.fBias;					// Store weight's influence over a vertex

							fileIn >> strCheck;						// Skip "("

							fileIn >> tempWeight.position.x					// Store weight's pos in joint's local space
								>> tempWeight.position.z
								>> tempWeight.position.y;

							tempVertexGroup.vtWeight.push_back(tempWeight);

							std::getline(fileIn, strCheck);			// Skip rest of this line
						}
					}
					else
					{
						std::getline(fileIn, strCheck);
					}

					fileIn >> strCheck;
				}

				if (bFailed == false)
					vtTemp.push_back(tempVertexGroup);
			}
		}
	}

	if (vtTemp.size() != nNumOfSubsets)
	{
		SAFE_DELETE(pModelData);
	}
	else
	{
		for(size_t i=0;i< vtTemp.size();++i)
		{
			MeshSubsets* pSubsets = new MeshSubsets;
			
			TempVertexGroup tempGroup = vtTemp[i];

			for (int j = 0; j < tempGroup.vtWeightCount.size(); ++j)
			{
				Vertex tempVertex;

				for (int k = 0; k < (int)tempGroup.vtWeightCount[j]; ++k)
				{
					int nindex = tempGroup.vtStartWeights[j] + k;

					Weight tempWeight = tempGroup.vtWeight[nindex];
					Joint tempJoint = pModelData->vtJoints[tempWeight.nJointID];

					DirectX::XMVECTOR tempJointOrientation = 
						DirectX::XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
					DirectX::XMVECTOR tempWeightPos = 
						DirectX::XMVectorSet(tempWeight.position.x, tempWeight.position.y, tempWeight.position.z, 0.0f);

					DirectX::XMVECTOR tempJointOrientationConjugate =
						DirectX::XMVectorSet(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

					DirectX::XMFLOAT3 rotatePoint;
					DirectX::XMStoreFloat3(&rotatePoint, 
						DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

					tempVertex.position.x += (tempJoint.position.x + rotatePoint.x) * tempWeight.fBias;
					tempVertex.position.y += (tempJoint.position.y + rotatePoint.y) * tempWeight.fBias;
					tempVertex.position.z += (tempJoint.position.z + rotatePoint.z) * tempWeight.fBias;
				}

				tempVertex.UV = tempGroup.vtUVs[j];

				pSubsets->vtVertices.push_back(tempVertex);
			}

			for (int j = 0; j < (int)tempGroup.vtIndicies.size(); ++j)
				pSubsets->vtIndicies.push_back(tempGroup.vtIndicies[j]);

			for (int j = 0; j < (int)tempGroup.vtWeight.size(); ++j)
				pSubsets->vtWeights.push_back(tempGroup.vtWeight[j]);

			//caculate normal
			std::vector<DirectX::XMFLOAT3> tempNormal;

			//normalized and unnormalized normals
			DirectX::XMFLOAT3 unnormalized = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

			//Used to get vectors (sides) from the position of the verts
			float vecX, vecY, vecZ;

			//Two edges of our triangle
			DirectX::XMVECTOR edge1 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR edge2 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//Compute face normals
			for (int i = 0; i < tempGroup.nNumTri; ++i)
			{
				//Get the vector describing one edge of our triangle (edge 0,2)
				vecX = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3)]].position.x - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.x;
				vecY = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3)]].position.y - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.y;
				vecZ = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3)]].position.z - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.z;
				edge1 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

				//Get the vector describing another edge of our triangle (edge 2,1)
				vecX = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.x - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 1]].position.x;
				vecY = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.y - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 1]].position.y;
				vecZ = pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 2]].position.z - pSubsets->vtVertices[pSubsets->vtIndicies[(i * 3) + 1]].position.z;
				edge2 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

				//Cross multiply the two edge vectors to get the un-normalized face normal
				DirectX::XMStoreFloat3(&unnormalized, DirectX::XMVector3Cross(edge1, edge2));

				tempNormal.push_back(unnormalized);
			}

			//Compute vertex normals (normal Averaging)
			DirectX::XMVECTOR normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			int facesUsing = 0;
			float tX, tY, tZ;	//temp axis variables

			//Go through each vertex
			for (int i = 0; i < pSubsets->vtVertices.size(); ++i)
			{
				//Check which triangles use this vertex
				for (int j = 0; j < tempGroup.nNumTri; ++j)
				{
					if (pSubsets->vtIndicies[j * 3] == i ||
						pSubsets->vtIndicies[(j * 3) + 1] == i ||
						pSubsets->vtIndicies[(j * 3) + 2] == i)
					{
						tX = DirectX::XMVectorGetX(normalSum) + tempNormal[j].x;
						tY = DirectX::XMVectorGetY(normalSum) + tempNormal[j].y;
						tZ = DirectX::XMVectorGetZ(normalSum) + tempNormal[j].z;

						normalSum = DirectX::XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum

						facesUsing++;
					}
				}

				//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
				normalSum = normalSum / facesUsing;

				//Normalize the normalSum vector
				normalSum = DirectX::XMVector3Normalize(normalSum);

				//Store the normal and tangent in our current vertex
				pSubsets->vtVertices[i].normal.x = -DirectX::XMVectorGetX(normalSum);
				pSubsets->vtVertices[i].normal.y = -DirectX::XMVectorGetY(normalSum);
				pSubsets->vtVertices[i].normal.z = -DirectX::XMVectorGetZ(normalSum);

				//Clear normalSum, facesUsing for next vertex
				normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				facesUsing = 0;
			}

			pSubsets->strMtlName = tempGroup.strMtlName;
			pSubsets->strSubsetName = tempGroup.strGroupName;

			pModelData->vtMeshSubsets.push_back(pSubsets);
		}

		MaterialData* pMaterialData = new MaterialData();

		std::map< std::basic_string<TCHAR>, std::basic_string<TCHAR> >::iterator iter;
		
		for ( iter = mapMaterials.begin();	iter != mapMaterials.end(); iter++)
		{
			MaterialData::MaterialInfo info;

			info.strDiffuseMap = iter->second;
			info.strName = iter->first;
			
			info.diffuse  = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			info.ambient  = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
			info.specular = DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1.0f);

			info.fSpecularDistance = 10.0f;

			pMaterialData->vtMaterialInfo.push_back(info);
		}

		pModelData->pMaterialData = pMaterialData;
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

			pData->vtMaterialInfo.push_back(*pInfo);
		}

		for (int i = 0; i < vtInfos.size(); ++i)
		{
			SAFE_DELETE(vtInfos[i]);
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
