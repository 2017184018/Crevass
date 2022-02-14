#include "Converter.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

FbxManager* gFbxManager = nullptr;

void FBXLoader::LoadFBX(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName)
{
	//Create Manager
	if (gFbxManager == nullptr) {
		gFbxManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT);
		gFbxManager->SetIOSettings(pIOsettings);
	}

	//Create importer
	FbxImporter* pImporter = FbxImporter::Create(gFbxManager, "");
	std::string fbxFileName = fileName + ".fbx";
	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings());
	if (!bSuccess) {
		std::cout << "Importer errer" << std::endl;
		exit(-1);
	}

	//Create Scene
	FbxScene* pFbxScene = FbxScene::Create(gFbxManager, "");
	bSuccess = pImporter->Import(pFbxScene);

	if (!bSuccess) {
		std::cout << "Scene errer" << std::endl;
		return;
	}
	pImporter->Destroy();
	
	//좌표축 가져오기
	//FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	//씬 내의 좌표축 바꿈
	//FbxAxisSystem::MayaZUp.ConvertScene(pFbxScene); 
	
	//씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킴 
	//FbxGeometryConverter geometryConverter(gFbxManager);
	//geometryConverter.Triangulate(pFbxScene, true);

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	LoadNode(pFbxRootNode, outVertexVector, outIndexVector);

	ExportMesh(outVertexVector, outIndexVector, outMaterial, fileName);

	return;
}

void FBXLoader::LoadNode(FbxNode* node, std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector)
{
	const int childCount = node->GetChildCount();
	if (node)
	{
		if (node->GetNodeAttribute() != NULL)
		{
				FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();
			
				FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();
				
				//std::cout << "타입이름이 뭐니 : " << FbxNodeAttribute::eNull << std::endl;
				//이 노드의 속성이 메쉬라면 
				if (AttributeType == FbxNodeAttribute::eMesh)
				{
					GetControlPoints(node);

					GetVerticesAndIndice(pMesh, outVertexVector, outIndexVector);

				}
			
		}
	}

	for (int i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i), outVertexVector, outIndexVector);
	}
}

bool FBXLoader::LoadMesh(std::string fileName, std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>* outMaterial)
{


	return false;
}

void FBXLoader::GetControlPoints(fbxsdk::FbxNode* pFbxRootNode)
{
	FbxMesh* pCurrMesh = (FbxMesh*)pFbxRootNode->GetNodeAttribute();

	unsigned int ctrlPointCount = pCurrMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; i++) {
		CtrlPoint* currCtrlPoint = new CtrlPoint();

		DirectX::XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[2]);

		currCtrlPoint->mPosition = currPosition;
		//i번째 컨트롤 포인트의 좌표정보 
		mControlPoints[i] = currCtrlPoint;
	}

}

void FBXLoader::GetVerticesAndIndice(fbxsdk::FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector)
{
	std::unordered_map<Vertex, uint32_t> IndexMap;
	uint32_t VertexIndex = 0;
	int PCount = pMesh->GetPolygonCount();

	for (int i = 0; i < PCount; ++i) {

		//Vertex and Index info 
		for (int j = 0; j < 3; ++j) {
			//i번째 폴리곤의 j번째 버텍스의 인덱스 
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			CtrlPoint* CurrCtrlPoint = mControlPoints[controlPointIndex];

			//노멀
			FbxVector4 pNormal;
			pMesh->GetPolygonVertexNormal(i, j, pNormal);

			//UV
			float* IUVs = NULL;
			FbxStringList lUVNames;
			pMesh->GetUVSetNames(lUVNames);
			const char* IUVName = NULL;
			if (lUVNames.GetCount())
			{
				IUVName = lUVNames[0];
			}

			FbxVector2 pUVs;
			bool bUnMappedUV;
			if (!pMesh->GetPolygonVertexUV(i, j, IUVName, pUVs, bUnMappedUV))
			{
				std::cout << "UV not found" << std::endl;
			}

			Vertex Temp;
			// Position
			Temp.Pos.x = CurrCtrlPoint->mPosition.x;
			Temp.Pos.y = CurrCtrlPoint->mPosition.y;
			Temp.Pos.z = CurrCtrlPoint->mPosition.z;

			// Normal
			Temp.Normal.x = pNormal.mData[0];
			Temp.Normal.y = pNormal.mData[1];
			Temp.Normal.z = pNormal.mData[2];

			// UV
			Temp.TexC.x = pUVs.mData[0];
			Temp.TexC.y = 1.0f - pUVs.mData[1];

			auto lookup = IndexMap.find(Temp);
			if (lookup != IndexMap.end()) {
				//Index
				outIndexVector.push_back(lookup->second);

			}
			else
			{
				//Index
				outIndexVector.push_back(VertexIndex);
				IndexMap[Temp] = VertexIndex;

				VertexIndex++;
				outVertexVector.push_back(Temp);
			}
		}
	}

}

void FBXLoader::GetMaterials(fbxsdk::FbxNode* pNode, std::vector<Material>& outMaterial)
{
	int MaterialCount = pNode->GetMaterialCount();

	for (int i = 0; i < MaterialCount; ++i) {
		Material tempMaterial;
		FbxSurfaceMaterial* SurfaceMaterial = pNode->GetMaterial(i);
		//GetMaterialAttribute(SurfaceMaterial, tempMaterial);
	//	GetMaterialTexture(SurfaceMaterial, tempMaterial);

		if (tempMaterial.Name != "")
		{
			outMaterial.push_back(tempMaterial);
		}
	}
}

//void FBXLoader::GetMaterialAttribute(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& outMaterial)
//{
//	FbxDouble3 double3;
//	FbxDouble double1;
//	if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
//	{
//		// Amibent Color
//		double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Ambient;
//		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
//		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
//		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);
//
//		// Diffuse Color
//		double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Diffuse;
//		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
//		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
//		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);
//
//		// Roughness 
//		double1 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Shininess;
//		outMaterial.Roughness = 1 - double1;
//
//		// Reflection
//		double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Reflection;
//		outMaterial.FresnelR0.x = static_cast<float>(double3.mData[0]);
//		outMaterial.FresnelR0.y = static_cast<float>(double3.mData[1]);
//		outMaterial.FresnelR0.z = static_cast<float>(double3.mData[2]);
//
//		// Specular Color
//		double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Specular;
//		outMaterial.Specular.x = static_cast<float>(double3.mData[0]);
//		outMaterial.Specular.y = static_cast<float>(double3.mData[1]);
//		outMaterial.Specular.z = static_cast<float>(double3.mData[2]);
//
//		// Emissive Color
//		double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Emissive;
//		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
//		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
//		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);
//	}
//	else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
//	{
//		// Amibent Color
//		double3 = reinterpret_cast<FbxSurfaceLambert*>(pMaterial)->Ambient;
//		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
//		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
//		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);
//
//		// Diffuse Color
//		double3 = reinterpret_cast<FbxSurfaceLambert*>(pMaterial)->Diffuse;
//		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
//		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
//		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);
//
//		// Emissive Color
//		double3 = reinterpret_cast<FbxSurfaceLambert*>(pMaterial)->Emissive;
//		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
//		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
//		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);
//	}
//
//}

//void FBXLoader::GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& Mat)
//{
//	unsigned int textureIndex = 0;
//	FbxProperty property;
//
//	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
//	{
//		property = pMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
//		if (property.IsValid())
//		{
//			unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
//			for (unsigned int i = 0; i < textureCount; ++i)
//			{
//				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
//				if (layeredTexture)
//				{
//					throw std::exception("Layered Texture is currently unsupported\n");
//				}
//				else
//				{
//
//					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
//					if (texture)
//					{
//						std::string textureType = property.GetNameAsCStr();
//						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
//
//						if (fileTexture)
//						{
//							if (textureType == "DiffuseColor")
//							{
//								Mat.Name = fileTexture->GetFileName();
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}

void FBXLoader::ExportMesh(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName)
{

	std::ofstream fileOut(fileName + ".mesh");

	if (outVertexVector.empty() || outIndexVector.empty())
		return;

	if (fileOut)
	{
		//제질은 일단 생략
		uint32_t vertexSize = outVertexVector.size();
		uint32_t indexSize = outIndexVector.size();
		uint32_t materialSize = outMaterial.size();

		fileOut << "VertexSize " << vertexSize << "\n";
		fileOut << "IndexSize " << indexSize << "\n";
		//fileOut << "MaterialSize " << materialSize << "\n";

		//fileOut << "Material " << "\n";
		for (auto& e : outMaterial)
		{
			fileOut << "Name" << e.Name << "\n";
			fileOut << "Ambient " << e.Ambient.x << " " << e.Ambient.y << " " << e.Ambient.z << "\n";
			fileOut << "Diffuse " << e.DiffuseAlbedo.x << " " << e.DiffuseAlbedo.y << " " << e.DiffuseAlbedo.z << " " << e.DiffuseAlbedo.w << "\n";
			fileOut << "Fresnel " << e.FresnelR0.x << " " << e.FresnelR0.y << " " << e.FresnelR0.z << "\n";
			fileOut << "Specular " << e.Specular.x << " " << e.Specular.y << " " << e.Specular.z << "\n";
			fileOut << "Emissive " << e.Emissive.x << " " << e.Emissive.y << " " << e.Emissive.z << "\n";
			fileOut << "Roughness " << e.Roughness << "\n";
			fileOut << "MatTransform ";
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileOut << e.MatTransform.m[i][j] << " ";
				}
			}
			fileOut << "\n";
		}

		fileOut << "Vertex " << "\n";
		for (auto& e : outVertexVector)
		{
			fileOut << "Pos " << e.Pos.x << " " << e.Pos.y << " " << e.Pos.z << "\n";
			fileOut << "Normal " << e.Normal.x << " " << e.Normal.y << " " << e.Normal.z << "\n";
			fileOut << "TexC " << e.TexC.x << " " << e.TexC.y << "\n";

		}

		fileOut << "Index " << "\n";
		for (uint32_t i = 0; i < indexSize / 3; i++) {

			fileOut << outIndexVector[3 * i] << " " << outIndexVector[3 * i + 1] << " " << outIndexVector[3 * i + 2] << "\n";

		}

	}
}
