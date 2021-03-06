#include <iostream>
#include <vector>
#include <winerror.h>
#include <assert.h>
// #include "FrameResource.h"
#include "vertexHash.h"
#include "FbxLoader.h"

//using namespace fbxsdk;
using namespace DirectX;

FbxLoader::FbxLoader()
{
}

FbxLoader::~FbxLoader()
{
	
}

FbxManager * gFbxManager = nullptr;

HRESULT FbxLoader::LoadFBX(
	std::vector<CharacterVertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	SkinnedData& outSkinnedData,
	const std::string& clipName,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	// if exported animation exist


	if (gFbxManager == nullptr)
	{
		gFbxManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT);
		gFbxManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(gFbxManager, "");
	std::string fbxFileName = fileName + clipName + ".fbx";
	//std::string fbxFileName = fileName  + ".fbx";
	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	FbxScene* pFbxScene = FbxScene::Create(gFbxManager, "");
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;
	pImporter->Destroy();
	
	fbxsdk::FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	fbxsdk::FbxAxisSystem::DirectX.ConvertScene(pFbxScene); // Delete?

	// Convert quad to triangle
	FbxGeometryConverter geometryConverter(gFbxManager);
	geometryConverter.Triangulate(pFbxScene, true);
	
	// Start to RootNode
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();


	LoadNode_sklt(pFbxRootNode, outSkinnedData, *pFbxScene, clipName);
	mBoneName = outSkinnedData.GetBoneName();
	mBoneHierarchy.size();
	LoadNode_AnimationAndMesh(pFbxRootNode, outVertexVector, outIndexVector, outSkinnedData, *pFbxScene, clipName,outMaterial);
	outSkinnedData.Set(mBoneHierarchy, mBoneOffsets, &mAnimations);
	
	// Export cmesh / skeleton / anim
	ExportMesh(outVertexVector, outIndexVector, outMaterial, fileName + clipName);
	
	ExportSkeleton(outSkinnedData, clipName, fileName);
	
	ExportAnimation(mAnimations[clipName], fileName, clipName);

	return S_OK;
}

HRESULT FbxLoader::LoadFBX(
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	// if exported animation exist

	if (gFbxManager == nullptr)
	{
		gFbxManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT);
		gFbxManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(gFbxManager, "");
	std::string fbxFileName = fileName + ".fbx";
	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	FbxScene* pFbxScene = FbxScene::Create(gFbxManager, "");
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;

	pImporter->Destroy();

	fbxsdk::FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	fbxsdk::FbxAxisSystem::DirectX.ConvertScene(pFbxScene); // Delete?

													// Convert quad to triangle
	FbxGeometryConverter geometryConverter(gFbxManager);
	geometryConverter.Triangulate(pFbxScene, true);

	// Start to RootNode
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	LoadNode(pFbxRootNode, outVertexVector, outIndexVector, outMaterial);

	ExportMesh(outVertexVector, outIndexVector, outMaterial, fileName);

	return S_OK;
}

HRESULT FbxLoader::LoadFBX(
	SkinnedData& outSkinnedData,
	const std::string& clipName,
	std::string fileName)
{
	// if exported animation exist
	if (LoadAnimation(outSkinnedData, clipName, fileName)) return S_OK;

	mBoneName = outSkinnedData.GetBoneName();

	if (gFbxManager == nullptr)
	{
		gFbxManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT);
		gFbxManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(gFbxManager, "");
	std::string fbxFileName = fileName + clipName + ".fbx";

	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, gFbxManager->GetIOSettings());
	if (!bSuccess) return E_FAIL;

	FbxScene* pFbxScene = FbxScene::Create(gFbxManager, "");
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return E_FAIL;

	pImporter->Destroy();

	fbxsdk::FbxAxisSystem sceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	fbxsdk::FbxAxisSystem::DirectX.ConvertScene(pFbxScene); // Delete?
	

	// Convert quad to triangle
	FbxGeometryConverter geometryConverter(gFbxManager);
	geometryConverter.Triangulate(pFbxScene, true);

	// Start to RootNode
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	if (pFbxRootNode)
	{
		// Animation Data
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();
			if (!pMesh || !AttributeType) { continue; }

			if (AttributeType == FbxNodeAttribute::eMesh)
			{
				// Get Animation Clip
				//GetOnlyAnimation(pFbxScene, pFbxChildNode, outSkinnedData, clipName);
				GetAnimation(pFbxScene, pFbxChildNode, outSkinnedData, clipName, true);
			}
		}
	}

	ExportAnimation(outSkinnedData.GetAnimation(clipName), fileName, clipName);
	return S_OK;
}

void FbxLoader::LoadNode(FbxNode* node, std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial)
{
	const int childCount = node->GetChildCount();

	if (node)
	{
		if (node->GetNodeAttribute() != NULL)
		{
			FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();

			FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();
			if (AttributeType == FbxNodeAttribute::eMesh)
			{
				GetControlPoints(node);

				GetVerticesAndIndice(pMesh, outVertexVector, outIndexVector);

				GetMaterials(node, outMaterial);

			}

		}
	}

	for (int i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i), outVertexVector, outIndexVector, outMaterial);
	}
}
int count = 0;
void FbxLoader::LoadNode_sklt(FbxNode* node, SkinnedData& outSkinnedData, FbxScene& pFbxScene, std::string clipName)
{
	const int childCount = node->GetChildCount();
	if (node)
	{
		if (node->GetNodeAttribute() != NULL)
		{
			FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();

			FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();

			switch (AttributeType)
			{
			case FbxNodeAttribute::eSkeleton:
				if (count == 0) {
					std::cout << "??d??" << std::endl;
					GetSkeletonHierarchy(node, outSkinnedData, 0, -1);
				}
				count++;
				break;
			}

		}
	}

	for (int i = 0; i < childCount; ++i)
	{
		LoadNode_sklt(node->GetChild(i), outSkinnedData, pFbxScene, clipName);

	}

}

void FbxLoader::LoadNode_AnimationAndMesh(FbxNode* node, std::vector<CharacterVertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, SkinnedData& outSkinnedData, FbxScene& pFbxScene, std::string clipName, std::vector<Material>& outMaterial)
{
	const int childCount = node->GetChildCount();
	if (node)
	{
		if (node->GetNodeAttribute() != NULL)
		{
			FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();
			FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();

			if (AttributeType == FbxNodeAttribute::eMesh)
			{
				std::cout << "??????" << std::endl;
				GetControlPoints(node);

				// To access the bone index directly
				mBoneOffsets.resize(mBoneHierarchy.size());

				// Get Animation Clip
				GetAnimation(&pFbxScene, node, outSkinnedData, clipName, false);
			

				// Get Vertices and indices info
				GetVerticesAndIndice(pMesh, outVertexVector, outIndexVector, &outSkinnedData);

 				GetMaterials(node, outMaterial);

			}
		}
	}


	for (int i = 0; i < childCount; ++i)
	{
		LoadNode_AnimationAndMesh(node->GetChild(i), outVertexVector, outIndexVector, outSkinnedData, pFbxScene, clipName, outMaterial);

	}
}

bool FbxLoader::LoadSkeleton(
	SkinnedData& outSkinnedData, 
	const std::string& clipName, 
	std::string fileName)
{
	fileName = fileName + clipName + ".skeleton";
	std::ifstream fileIn(fileName);

	uint32_t boneSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> boneSize;

		if (boneSize == 0)
			return false;

		// Bone Data
		// Bone Hierarchy
		fileIn >> ignore;
		std::vector<int> boneHierarchy;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			int tempBoneHierarchy;
			fileIn >> tempBoneHierarchy;
			boneHierarchy.push_back(tempBoneHierarchy);
		}
		
		fileIn >> ignore;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			std::string tempBoneName;
			fileIn >> tempBoneName;
			outSkinnedData.SetBoneName(tempBoneName);
		}
		// Bone Offset
		fileIn >> ignore;
		std::vector<DirectX::XMFLOAT4X4> boneOffsets;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			DirectX::XMFLOAT4X4 tempBoneOffset;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileIn >> tempBoneOffset.m[i][j];
				}
			}
			boneOffsets.push_back(tempBoneOffset);
		}
		// Bone Submesh Offset
		fileIn >> ignore;
		std::vector<int> boneSubmeshOffset;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			int tempBoneSubmeshOffset;
			fileIn >> tempBoneSubmeshOffset;
			outSkinnedData.SetSubmeshOffset(tempBoneSubmeshOffset);
		}

		outSkinnedData.Set(
			boneHierarchy,
			boneOffsets);

		return true;
	}

	return false;
}

bool FbxLoader::LoadMesh(
	std::string fileName,
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>* outMaterial)
{
	fileName = fileName + ".mesh";
	std::ifstream fileIn(fileName);

	uint32_t vertexSize, indexSize;
	uint32_t materialSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> vertexSize;
		fileIn >> ignore >> indexSize;
		fileIn >> ignore >> materialSize;

		if (vertexSize == 0 || indexSize == 0	)
			return false;

		// Material Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < materialSize; ++i)
		{
			Material tempMaterial;

			fileIn >> ignore >> tempMaterial.Name;
			fileIn >> ignore >> tempMaterial.Ambient.x >> tempMaterial.Ambient.y >> tempMaterial.Ambient.z;
			fileIn >> ignore >> tempMaterial.DiffuseAlbedo.x >> tempMaterial.DiffuseAlbedo.y >> tempMaterial.DiffuseAlbedo.z >> tempMaterial.DiffuseAlbedo.w;
			fileIn >> ignore >> tempMaterial.FresnelR0.x >> tempMaterial.FresnelR0.y >> tempMaterial.FresnelR0.z;
			fileIn >> ignore >> tempMaterial.Specular.x >> tempMaterial.Specular.y >> tempMaterial.Specular.z;
			fileIn >> ignore >> tempMaterial.Emissive.x >> tempMaterial.Emissive.y >> tempMaterial.Emissive.z;
			fileIn >> ignore >> tempMaterial.Roughness;
			fileIn >> ignore;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileIn >> tempMaterial.MatTransform.m[i][j];
				}
			}
			(*outMaterial).push_back(tempMaterial);
		}

		// Vertex Data
		for (uint32_t i = 0; i < vertexSize; ++i)
		{
			Vertex vertex;
			fileIn >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
			fileIn >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
			fileIn >> ignore >> vertex.TexC.x >> vertex.TexC.y;
			fileIn >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
			fileIn >> ignore >> vertex.Binormal.x >> vertex.Binormal.y >> vertex.Binormal.z;

			// push_back
			outVertexVector.push_back(vertex);
		}

		// Index Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < indexSize; ++i)
		{
			uint32_t index;
			fileIn >> index;
			outIndexVector.push_back(index);
		}
		
		return true;
	}

	return false;
}

bool FbxLoader::LoadMesh(
	std::string fileName,
	std::vector<CharacterVertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>* outMaterial)
{
	fileName = fileName + ".cmesh";
	std::ifstream fileIn(fileName);

	uint32_t vertexSize, indexSize;
	uint32_t materialSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> vertexSize;
		fileIn >> ignore >> indexSize;
		fileIn >> ignore >> materialSize;

		if (vertexSize == 0 || indexSize == 0)
			return false;

		if (outMaterial != nullptr)
		{
			// Material Data
			fileIn >> ignore;
			for (uint32_t i = 0; i < materialSize; ++i)
			{
				Material tempMaterial;

				fileIn >> ignore >> tempMaterial.Name;
				fileIn >> ignore >> tempMaterial.Ambient.x >> tempMaterial.Ambient.y >> tempMaterial.Ambient.z;
				fileIn >> ignore >> tempMaterial.DiffuseAlbedo.x >> tempMaterial.DiffuseAlbedo.y >> tempMaterial.DiffuseAlbedo.z >> tempMaterial.DiffuseAlbedo.w;
				fileIn >> ignore >> tempMaterial.FresnelR0.x >> tempMaterial.FresnelR0.y >> tempMaterial.FresnelR0.z;
				fileIn >> ignore >> static_cast<float>(tempMaterial.Specular.x) >> tempMaterial.Specular.y >> tempMaterial.Specular.z;
				fileIn >> ignore >> tempMaterial.Emissive.x >> tempMaterial.Emissive.y >> tempMaterial.Emissive.z;
				fileIn >> ignore >> tempMaterial.Roughness;
				fileIn >> ignore;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						fileIn >> tempMaterial.MatTransform.m[i][j];
					}
				}
				(*outMaterial).push_back(tempMaterial);
			}
		}
		
		// Vertex Data
		for (uint32_t i = 0; i < vertexSize; ++i)
		{
			CharacterVertex vertex;
			int temp[4];
			fileIn >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
			fileIn >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
			fileIn >> ignore >> vertex.TexC.x >> vertex.TexC.y;
			fileIn >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
			fileIn >> ignore >> vertex.Binormal.x >> vertex.Binormal.y >> vertex.Binormal.z;
			fileIn >> ignore >> vertex.BoneWeights.x >> vertex.BoneWeights.y >> vertex.BoneWeights.z;
			fileIn >> ignore >> temp[0] >> temp[1] >> temp[2] >> temp[3];

			for (int j = 0; j < 4; ++j)
			{
				vertex.BoneIndices[j] = temp[j];
			}
			// push_back
			outVertexVector.push_back(vertex);
		}

		// Index Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < indexSize; ++i)
		{
			uint32_t index;
			fileIn >> index;
			outIndexVector.push_back(index);
		}

		return true;
	}

	return false;
}

bool FbxLoader::LoadAnimation(
	SkinnedData& outSkinnedData,
	const std::string& clipName, 
	std::string fileName)
{
	fileName = fileName + clipName + ".anim";
	std::ifstream fileIn(fileName);

	AnimationClip animation;
	uint32_t boneAnimationSize, keyframeSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> boneAnimationSize;
		fileIn >> ignore >> keyframeSize;

		for (uint32_t i = 0; i < boneAnimationSize; ++i)
		{
			BoneAnimation boneAnim;
			for (uint32_t j = 0; j < keyframeSize; ++j)
			{
				Keyframe key;
				fileIn >> key.TimePos;
				fileIn >> key.Translation.x >> key.Translation.y >> key.Translation.z;
				fileIn >> key.Scale.x >> key.Scale.y >> key.Scale.z;
				fileIn >> key.RotationQuat.x >> key.RotationQuat.y >> key.RotationQuat.z >> key.RotationQuat.w;
				boneAnim.Keyframes.push_back(key);
			}
			animation.BoneAnimations.push_back(boneAnim);
		}

		outSkinnedData.SetAnimation(animation, clipName);
		return true;
	}

	return false;
}


void FbxLoader::GetSkeletonHierarchy(
	FbxNode * pNode,
	SkinnedData& outSkinnedData, 
	int curIndex, int parentIndex)
{
	mBoneHierarchy.push_back(parentIndex);
	outSkinnedData.SetBoneName(pNode->GetName());

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		GetSkeletonHierarchy(pNode->GetChild(i), outSkinnedData, mBoneHierarchy.size(), curIndex);
	}
}

void FbxLoader::GetControlPoints(FbxNode * pFbxRootNode)
{
	FbxMesh * pCurrMesh = (FbxMesh*)pFbxRootNode->GetNodeAttribute();

	unsigned int ctrlPointCount = pCurrMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();

		DirectX::XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(pCurrMesh->GetControlPointAt(i).mData[2]);

		currCtrlPoint->mPosition = currPosition;
		mControlPoints[i] = currCtrlPoint;
	}
}


void FbxLoader::GetAnimation(
	FbxScene* pFbxScene,
	FbxNode * pFbxChildNode,
	SkinnedData& outSkinnedData,
	const std::string& ClipName,
	bool isGetOnlyAnim)
{

	FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
	FbxAMatrix geometryTransform = GetGeometryTransformation(pFbxChildNode);


	// Animation Data
	AnimationClip animation;

	// Initialize BoneAnimations
	animation.BoneAnimations.resize(mBoneName.size());

	// Deformer - Cluster - Link
	// Deformer
	for (int deformerIndex = 0; deformerIndex < pMesh->GetDeformerCount(); ++deformerIndex)
	{
		FbxSkin* pCurrSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!pCurrSkin) { continue; }

		// Cluster
		for (int clusterIndex = 0; clusterIndex < pCurrSkin->GetClusterCount(); ++clusterIndex)
		{
			FbxCluster* pCurrCluster = pCurrSkin->GetCluster(clusterIndex);

			// To find the index that matches the name of the current joint
			std::string currJointName = pCurrCluster->GetLink()->GetName();
			BYTE currJointIndex; // current joint index
			for (currJointIndex = 0; currJointIndex < mBoneName.size(); ++currJointIndex)
			{
				if (mBoneName[currJointIndex] == currJointName)
					break;
			}


			if (!isGetOnlyAnim)
			{
				FbxAMatrix transformMatrix, transformLinkMatrix;
				FbxAMatrix globalBindposeInverseMatrix;

				transformMatrix = pCurrCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
				transformLinkMatrix = pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

				// Set the BoneOffset Matrix
				DirectX::XMFLOAT4X4 boneOffset;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						boneOffset.m[i][j] = globalBindposeInverseMatrix.Get(i, j);
					}
				}
				mBoneOffsets[currJointIndex] = boneOffset;

				// Set the Bone index and weight ./ Max 4
				auto controlPointIndices = pCurrCluster->GetControlPointIndices();
				for (int i = 0; i < pCurrCluster->GetControlPointIndicesCount(); ++i)
				{
					BoneIndexAndWeight currBoneIndexAndWeight;
					currBoneIndexAndWeight.mBoneIndices = currJointIndex;
					currBoneIndexAndWeight.mBoneWeight = pCurrCluster->GetControlPointWeights()[i];

					mControlPoints[controlPointIndices[i]]->mBoneInfo.push_back(currBoneIndexAndWeight);
					mControlPoints[controlPointIndices[i]]->mBoneName = currJointName;
				}
			}

			
			// Set the Bone Animation Matrix
			BoneAnimation boneAnim;
			//0: Idle_A 1: Clicked 2: Bounce 3: Idle_B 4: Idle_C 5: Walk 6:Run 7:Fly 8: Peck 9:Roll 10: Spin 11:Death 12:Fear 13:Jump
			FbxAnimStack* pCurrAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = pCurrAnimStack->GetName();
			FbxTakeInfo* takeInfo = pFbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			pFbxScene->SetCurrentAnimationStack(pCurrAnimStack);
			FbxAnimStack* pCurrAnimStack2 = pFbxScene->GetCurrentAnimationStack();
			FbxString animStackName2 = pCurrAnimStack2->GetName();
			std::cout << "???? animation name == " << animStackName2 << std::endl;
			FbxAnimEvaluator* pSceneEvaluator = pFbxScene->GetAnimationEvaluator();
	
			// TRqS transformation and Time per frame
			FbxLongLong index;
			for (index = start.GetFrameCount(FbxTime::eFrames24); index <11; ++index)
			{
				FbxTime currTime;
				currTime.SetFrame(index, FbxTime::eFrames24);

				Keyframe key;
				key.TimePos = static_cast<float>(index) / 8.0f;
				FbxAMatrix currentTransformOffset = pSceneEvaluator->GetNodeGlobalTransform(pFbxChildNode, currTime) * geometryTransform;
				FbxAMatrix temp = currentTransformOffset.Inverse() * pSceneEvaluator->GetNodeGlobalTransform(pCurrCluster->GetLink(), currTime);

				// Transition, Scaling and Rotation Quaternion
				FbxVector4 TS = temp.GetT();
				key.Translation = {
					static_cast<float>(TS.mData[0]),
					static_cast<float>(TS.mData[1]),
					static_cast<float>(TS.mData[2]) };
				TS = temp.GetS();
				key.Scale = {
					static_cast<float>(TS.mData[0]),
					static_cast<float>(TS.mData[1]),
					static_cast<float>(TS.mData[2]) };
				FbxQuaternion Q = temp.GetQ();
				key.RotationQuat = {
					static_cast<float>(Q.mData[0]),
					static_cast<float>(Q.mData[1]),
					static_cast<float>(Q.mData[2]) ,
					static_cast<float>(Q.mData[3]) };

				// Frame does not exist
			/*	if (index != 0 && boneAnim.Keyframes.back() == key)
					break;*/

				boneAnim.Keyframes.push_back(key);
			}
			animation.BoneAnimations[currJointIndex] = boneAnim;
		}
	}

	BoneAnimation InitBoneAnim;

	// Initialize InitBoneAnim
	for (int i = 0; i < mBoneName.size(); ++i)
	{
		int KeyframeSize = animation.BoneAnimations[i].Keyframes.size();
		if (KeyframeSize != 0)
		{
			for (int j = 0; j < KeyframeSize; ++j) // 60 frames
			{
				Keyframe key;

				key.TimePos = static_cast<float>(j / 24.0f);
				key.Translation = { 0.0f, 0.0f, 0.0f };
				key.Scale = { 1.0f, 1.0f, 1.0f };
				key.RotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
				InitBoneAnim.Keyframes.push_back(key);
			}
			break;
		}
	}

	for (int i = 0; i < mBoneName.size(); ++i)
	{
		if (animation.BoneAnimations[i].Keyframes.size() != 0)
			continue;

		animation.BoneAnimations[i] = InitBoneAnim;
	}

	if (!isGetOnlyAnim)
	{
		BoneIndexAndWeight currBoneIndexAndWeight;
		currBoneIndexAndWeight.mBoneIndices = 0;
		currBoneIndexAndWeight.mBoneWeight = 0;
		for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
		{
			for (uint32_t i = itr->second->mBoneInfo.size(); i <= 4; ++i)
			{
				itr->second->mBoneInfo.push_back(currBoneIndexAndWeight);
			}
		}

		mAnimations[ClipName] = animation;
	}
	
	outSkinnedData.SetAnimation(animation, ClipName);
}

void FbxLoader::CalculateTangentBinormalVector(std::vector<Vertex>& vertexVector)
{
	// Temp Vector
	XMVECTOR v1 = XMVectorSet(
		vertexVector[1].Pos.x - vertexVector[0].Pos.x,
		vertexVector[1].Pos.y - vertexVector[0].Pos.y,
		vertexVector[1].Pos.z - vertexVector[0].Pos.z, 0.0f);
	XMVECTOR v2 = XMVectorSet(
		vertexVector[2].Pos.x - vertexVector[0].Pos.x,
		vertexVector[2].Pos.y - vertexVector[0].Pos.y,
		vertexVector[2].Pos.z - vertexVector[0].Pos.z, 0.0f);
	XMVECTOR u1x = XMVectorReplicate(vertexVector[1].TexC.x - vertexVector[0].TexC.x);
	XMVECTOR u1y = XMVectorReplicate(vertexVector[1].TexC.y - vertexVector[0].TexC.y);
	XMVECTOR u2x = XMVectorReplicate(vertexVector[2].TexC.x - vertexVector[0].TexC.x);
	XMVECTOR u2y = XMVectorReplicate(vertexVector[2].TexC.y - vertexVector[0].TexC.y);

	XMVECTOR det = XMVectorSubtract(XMVectorMultiply(u1x, u2y), XMVectorMultiply(u2x, u1y));

	XMVECTOR T = XMVectorDivide(XMVectorSubtract(XMVectorMultiply(v1, u2y), XMVectorMultiply(v2, u1y)), det);
	XMVECTOR T0 = XMLoadFloat3(&vertexVector[0].Tangent);
	XMVECTOR T1 = XMLoadFloat3(&vertexVector[0].Tangent);
	XMVECTOR T2 = XMLoadFloat3(&vertexVector[0].Tangent);
	T0 = XMVectorAdd(T0, T);
	T1 = XMVectorAdd(T1, T);
	T2 = XMVectorAdd(T2, T);

	XMVECTOR B = XMVectorDivide(XMVectorSubtract(XMVectorMultiply(v2, u1x), XMVectorMultiply(v1, u1x)), det);
	XMVECTOR B0 = XMLoadFloat3(&vertexVector[0].Binormal);
	XMVECTOR B1 = XMLoadFloat3(&vertexVector[0].Binormal);
	XMVECTOR B2 = XMLoadFloat3(&vertexVector[0].Binormal);
	B0 = XMVectorAdd(B0, B);
	B1 = XMVectorAdd(B1, B);
	B2 = XMVectorAdd(B2, B);

	XMStoreFloat3(&vertexVector[0].Tangent, T0);
	XMStoreFloat3(&vertexVector[1].Tangent, T1);
	XMStoreFloat3(&vertexVector[2].Tangent, T2);
	XMStoreFloat3(&vertexVector[0].Binormal, B0);
	XMStoreFloat3(&vertexVector[1].Binormal, B1);
	XMStoreFloat3(&vertexVector[2].Binormal, B2);
}

void FbxLoader::GetVerticesAndIndice(
	FbxMesh * pMesh, 
	std::vector<CharacterVertex> & outVertexVector, 
	std::vector<uint32_t> & outIndexVector,
	SkinnedData* outSkinnedData)
{
	// Vertex and Index
	std::unordered_map<std::string, std::vector<uint32_t>> IndexVector;
	std::unordered_map<Vertex, uint32_t> IndexMapping;
	uint32_t VertexIndex = 0;
	uint32_t tCount = pMesh->GetPolygonCount(); // Triangle

	for (uint32_t i = 0; i < tCount; ++i)
	{
		std::vector<Vertex> tVertex(3);
		std::vector<uint32_t> tIndex(3);

		// For indexing by bone
		std::string CurrBoneName = mControlPoints[pMesh->GetPolygonVertex(i, 1)]->mBoneName;

		// Vertex and Index info
		for (int j = 0; j < 3; ++j)
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			CtrlPoint* CurrCtrlPoint = mControlPoints[controlPointIndex];

			// Normal
			FbxVector4 pNormal;
			pMesh->GetPolygonVertexNormal(i, j, pNormal);

			// UV
			float * lUVs = NULL;
			FbxStringList lUVNames;
			pMesh->GetUVSetNames(lUVNames);
			const char * lUVName = NULL;
			if (lUVNames.GetCount())
			{
				lUVName = lUVNames[0];
			}

			FbxVector2 pUVs;
			bool bUnMappedUV;
			if (!pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV))
			{
				MessageBox(0, L"UV not found", 0, 0);
			}

			Vertex Temp;
			// Position
			Temp.Pos.x = CurrCtrlPoint->mPosition.x;
			Temp.Pos.y = CurrCtrlPoint->mPosition.y;
			Temp.Pos.z = CurrCtrlPoint->mPosition.z;

			// Normal
			Temp.Normal.x = static_cast<float>(pNormal.mData[0]);
			Temp.Normal.y = static_cast<float>(pNormal.mData[1]);
			Temp.Normal.z = static_cast<float>(pNormal.mData[2]);

			// UV
			Temp.TexC.x = static_cast<float>(pUVs.mData[0]);
			Temp.TexC.y = static_cast<float>(1.0f - pUVs.mData[1]);

			// push vertex and index
			auto lookup = IndexMapping.find(Temp);

			if (lookup != IndexMapping.end())
			{
				IndexVector[CurrBoneName].push_back(lookup->second);
				tIndex[j] = lookup->second;
			}
			else
			{
				// Index
				uint32_t Index = VertexIndex++;
				IndexMapping[Temp] = Index;
				IndexVector[CurrBoneName].push_back(Index);

				// Vertex
				CharacterVertex SkinnedVertexInfo;
				SkinnedVertexInfo.Pos = Temp.Pos;
				SkinnedVertexInfo.Normal = Temp.Normal;
				SkinnedVertexInfo.TexC = Temp.TexC;

				CurrCtrlPoint->SortBlendingInfoByWeight();

				// Set the Bone information
				for (int l = 0; l < CurrCtrlPoint->mBoneInfo.size(); ++l)
				{
					if (l >= 4)
						break;

					SkinnedVertexInfo.BoneIndices[l] = CurrCtrlPoint->mBoneInfo[l].mBoneIndices;

					switch (l)
					{
					case 0:
						SkinnedVertexInfo.BoneWeights.x = CurrCtrlPoint->mBoneInfo[l].mBoneWeight;
						break;
					case 1:
						SkinnedVertexInfo.BoneWeights.y = CurrCtrlPoint->mBoneInfo[l].mBoneWeight;
						break;
					case 2:
						SkinnedVertexInfo.BoneWeights.z = CurrCtrlPoint->mBoneInfo[l].mBoneWeight;
						break;
					}
				}

				tIndex[j] = outVertexVector.size();
				outVertexVector.push_back(SkinnedVertexInfo);
			}

			tVertex[j] = Temp;
		}

		// Calculate Tangent Vector and Binormal Vector
		// http://www.terathon.com/code/tangent.html
		int indexCounter = 0;
		for (uint32_t i = 0; i < tCount; ++i)
		{
			// CalculateTangentBinormalVector(tVertex);

			for (int j = 0; j < 3; ++j)
			{
				outVertexVector[tIndex[j]].Tangent = tVertex[j].Tangent;
				outVertexVector[tIndex[j]].Binormal = tVertex[j].Binormal;
			}
		}
	}

	for (uint32_t i = 0; i < outVertexVector.size(); ++i)
	{
		XMVECTOR N = XMLoadFloat3(&outVertexVector[i].Normal);
		XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&outVertexVector[i].Tangent));
		XMVECTOR B = XMVector3Normalize(XMLoadFloat3(&outVertexVector[i].Binormal));

		XMVECTOR T0 = XMVector3Normalize(XMVectorSubtract(T, XMVectorMultiply(N, XMVector3Dot(N, T))));
		XMVECTOR H = XMVectorReplicate((XMVector3Dot(XMVector3Cross(N, T), B).m128_f32[0] < 0.0f) ? -1.0f : 1.0f);
		XMVECTOR B0 = XMVectorMultiply(XMVector3Cross(N, T), H);

		XMStoreFloat3(&outVertexVector[i].Tangent, T0);
		XMStoreFloat3(&outVertexVector[i].Binormal, B0);
	}
	for (int i = 0; i < mBoneName.size(); ++i)
	{
		auto CurrIndexVector = IndexVector[mBoneName[i]];
		int IndexCount = CurrIndexVector.size();

		(*outSkinnedData).SetSubmeshOffset(IndexCount);

		outIndexVector.insert(outIndexVector.end(), CurrIndexVector.begin(), CurrIndexVector.end());
	}
}

void FbxLoader::GetVerticesAndIndice(
	FbxMesh * pMesh,
	std::vector<Vertex> & outVertexVector,
	std::vector<uint32_t> & outIndexVector)
{
	// Vertex and Index
	std::unordered_map<Vertex, uint32_t> IndexMapping;
	uint32_t VertexIndex = 0;
	int tCount = pMesh->GetPolygonCount(); // Triangle

	for (int i = 0; i < tCount; ++i)
	{
		std::vector<Vertex> tVertex(3);
		std::vector<uint32_t> tIndex(3);
		// Vertex and Index info
		for (int j = 0; j < 3; ++j)
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			CtrlPoint* CurrCtrlPoint = mControlPoints[controlPointIndex];

			// Normal
			FbxVector4 pNormal;
			pMesh->GetPolygonVertexNormal(i, j, pNormal);

			// UV
			float * lUVs = NULL;
			FbxStringList lUVNames;
			pMesh->GetUVSetNames(lUVNames);
			const char * lUVName = NULL;
			if (lUVNames.GetCount())
			{
				lUVName = lUVNames[0];
			}

			FbxVector2 pUVs;
			bool bUnMappedUV;
			if (!pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV))
			{
				MessageBox(0, L"UV not found", 0, 0);
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

			// push vertex and index
			auto lookup = IndexMapping.find(Temp);
			if (lookup != IndexMapping.end())
			{
				// Index
				outIndexVector.push_back(lookup->second);
				tIndex[j] = lookup->second;
			}
			else
			{
				// Index
				outIndexVector.push_back(VertexIndex);
				IndexMapping[Temp] = VertexIndex;

				VertexIndex++;
				tIndex[j] = outVertexVector.size();
				outVertexVector.push_back(Temp);
			}
			tVertex[j] = Temp;
		}

		// Calculate Tangent Vector and Binormal Vector
		// http://www.terathon.com/code/tangent.html
		for (uint32_t i = 0; i < tCount; ++i)
		{
			// CalculateTangentBinormalVector(tVertex);

			for (int j = 0; j < 3; ++j)
			{
				outVertexVector[tIndex[j]].Tangent = tVertex[j].Tangent;
				outVertexVector[tIndex[j]].Binormal = tVertex[j].Binormal;
			}
		}
	}

	
	for (uint32_t i = 0; i < outVertexVector.size(); ++i)
	{
		XMVECTOR N = XMLoadFloat3(&outVertexVector[i].Normal);
		XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&outVertexVector[i].Tangent));
		XMVECTOR B = XMVector3Normalize(XMLoadFloat3(&outVertexVector[i].Binormal));

		XMVECTOR T0 = XMVector3Normalize(XMVectorSubtract(T, XMVectorMultiply(N, XMVector3Dot(N, T))));
		XMVECTOR H = XMVectorReplicate((XMVector3Dot(XMVector3Cross(N, T), B).m128_f32[0] < 0.0f) ? -1.0f : 1.0f);
		XMVECTOR B0 = XMVectorMultiply(XMVector3Cross(N, T), H);

		XMStoreFloat3(&outVertexVector[i].Tangent, T0);
		XMStoreFloat3(&outVertexVector[i].Binormal, B0);
	}
}

void FbxLoader::GetMaterials(FbxNode* pNode, std::vector<Material>& outMaterial)
{
	int MaterialCount = pNode->GetMaterialCount();

	for (int i = 0; i < MaterialCount; ++i)
	{
		Material tempMaterial;
		FbxSurfaceMaterial* SurfaceMaterial = pNode->GetMaterial(i);
		GetMaterialAttribute(SurfaceMaterial, tempMaterial);
		GetMaterialTexture(SurfaceMaterial, tempMaterial);

		if (tempMaterial.Name != "")
		{
			outMaterial.push_back(tempMaterial);
		}
	}
}

void FbxLoader::GetMaterialAttribute(FbxSurfaceMaterial* pMaterial, Material& outMaterial)
{
	FbxDouble3 double3;
	FbxDouble double1;
	if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Ambient;
		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Diffuse;
		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);

		// Roughness 
		double1 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Shininess;
		outMaterial.Roughness = 1 - double1;

		// Reflection
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Reflection;
		outMaterial.FresnelR0.x = static_cast<float>(double3.mData[0]);
		outMaterial.FresnelR0.y = static_cast<float>(double3.mData[1]);
		outMaterial.FresnelR0.z = static_cast<float>(double3.mData[2]);

		// Specular Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Specular;
		outMaterial.Specular.x = static_cast<float>(double3.mData[0]);
		outMaterial.Specular.y = static_cast<float>(double3.mData[1]);
		outMaterial.Specular.z = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(pMaterial)->Emissive;
		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);

		/*
		// Transparency Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->TransparencyFactor;
		currMaterial->mTransparencyFactor = double1;

		// Specular Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->SpecularFactor;
		currMaterial->mSpecularPower = double1;


		// Reflection Factor
	double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->ReflectionFactor;
		currMaterial->mReflectionFactor = double1;	*/
	}
	else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Ambient;
		outMaterial.Ambient.x = static_cast<float>(double3.mData[0]);
		outMaterial.Ambient.y = static_cast<float>(double3.mData[1]);
		outMaterial.Ambient.z = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Diffuse;
		outMaterial.DiffuseAlbedo.x = static_cast<float>(double3.mData[0]);
		outMaterial.DiffuseAlbedo.y = static_cast<float>(double3.mData[1]);
		outMaterial.DiffuseAlbedo.z = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(pMaterial)->Emissive;
		outMaterial.Emissive.x = static_cast<float>(double3.mData[0]);
		outMaterial.Emissive.y = static_cast<float>(double3.mData[1]);
		outMaterial.Emissive.z = static_cast<float>(double3.mData[2]);
	}
}

void FbxLoader::GetMaterialTexture(fbxsdk::FbxSurfaceMaterial * pMaterial, Material & Mat)
{
	unsigned int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = pMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (unsigned int i = 0; i < textureCount; ++i)
			{
				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (layeredTexture)
				{
					throw std::exception("Layered Texture is currently unsupported\n");
				}
				else
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						std::string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							if (textureType == "DiffuseColor")
							{
								Mat.Name = fileTexture->GetFileName();
							}
							/*else if (textureType == "SpecularColor")
							{
							Mat->mSpecularMapName = fileTexture->GetFileName();
							}
							else if (textureType == "Bump")
							{
							Mat->mNormalMapName = fileTexture->GetFileName();
							}*/
						}
					}
				}
			}
		}
	}
}

int FbxLoader::GetAnimationLayerCurveNodes(FbxAnimLayer* pfbxAnimationLayer, FbxNode* pfbxNode)
{
	int nAnimationCurveNodes = 0;
	if (GetAnimationCurves(pfbxAnimationLayer, pfbxNode) > 0) nAnimationCurveNodes++;

	for (int i = 0; i < pfbxNode->GetChildCount(); i++)
	{
		nAnimationCurveNodes += GetAnimationLayerCurveNodes(pfbxAnimationLayer, pfbxNode->GetChild(i));
	}

	return(nAnimationCurveNodes);
}

int FbxLoader::GetAnimationCurves(FbxAnimLayer* pfbxAnimationLayer, FbxNode* pfbxNode)
{
	int nAnimationCurves = 0;

	FbxAnimCurve* pfbxAnimationCurve = NULL;
	if (pfbxAnimationCurve = pfbxNode->LclTranslation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_X)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclTranslation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Y)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclTranslation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Z)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclRotation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_X)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclRotation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Y)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclRotation.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Z)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclScaling.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_X)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclScaling.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Y)) nAnimationCurves++;
	if (pfbxAnimationCurve = pfbxNode->LclScaling.GetCurve(pfbxAnimationLayer, FBXSDK_CURVENODE_COMPONENT_Z)) nAnimationCurves++;

	return(nAnimationCurves);
}

FbxAMatrix FbxLoader::GetGeometryTransformation(FbxNode* pNode)
{
	if (!pNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}


void FbxLoader::ExportAnimation(
	const AnimationClip& animation, 
	std::string fileName, 
	const std::string& clipName)
{
	fileName = fileName + clipName + ".anim";
	std::ofstream fileOut(fileName);
	
	if (animation.BoneAnimations[1].Keyframes.size() == 0)
		return;
	int keyframessize = animation.BoneAnimations[1].Keyframes.size();
	
	for (int i = 0; i < animation.BoneAnimations.size(); i++) {
		std::cout << "???????? ?????? =" << animation.BoneAnimations[i].Keyframes.size() << std::endl;
	}

	if (fileOut)
	{
		uint32_t boneSize = animation.BoneAnimations.size();
		uint32_t keyframeSize = animation.BoneAnimations[1].Keyframes.size();
		fileOut << "Bone " << boneSize << "\n";
		fileOut << "KeframeSize " << keyframeSize << "\n";
		for (auto& e : animation.BoneAnimations)
		{
			if (e.Keyframes.size() == 1) {
				for (int i=0; i< keyframessize;i++)
				{
					for (auto& o : e.Keyframes)
					{

						fileOut << o.TimePos +i*0.125 << "\n";
						fileOut << o.Translation.x << " " << o.Translation.y << " " << o.Translation.z << "\n";
						fileOut << o.Scale.x << " " << o.Scale.y << " " << o.Scale.z << "\n";
						fileOut << o.RotationQuat.x << " " << o.RotationQuat.y << " " << o.RotationQuat.z << " " << o.RotationQuat.w << "\n";
					}
				}
			}
			else {
				for (auto& o : e.Keyframes)
				{

					fileOut << o.TimePos << "\n";
					fileOut << o.Translation.x << " " << o.Translation.y << " " << o.Translation.z << "\n";
					fileOut << o.Scale.x << " " << o.Scale.y << " " << o.Scale.z << "\n";
					fileOut << o.RotationQuat.x << " " << o.RotationQuat.y << " " << o.RotationQuat.z << " " << o.RotationQuat.w << "\n";
				}
			}
		}
	}
}

void FbxLoader::ExportSkeleton(
	SkinnedData& outSkinnedData, 
	const std::string& clipName, 
	std::string fileName)
{
	std::ofstream skeletonFileOut(fileName + clipName + ".skeleton");

	if (outSkinnedData.BoneCount() == 0)
		return;

	if (skeletonFileOut)
	{

		uint32_t boneSize = outSkinnedData.BoneCount();

		skeletonFileOut << "Bone " << boneSize << "\n";

		skeletonFileOut << "BoneHierarchy" << "\n";
		for (auto& e : outSkinnedData.GetBoneHierarchy())
		{
			skeletonFileOut << e << " ";
		}
		skeletonFileOut << "\n";

		skeletonFileOut << "BoneName" << "\n";
		for (auto& e : outSkinnedData.GetBoneName())
		{
			skeletonFileOut << e << " ";
		}
		skeletonFileOut << "\n";

		skeletonFileOut << "BoneOffset " << "\n";
		for (auto& e : outSkinnedData.GetBoneOffsets())
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					skeletonFileOut << e.m[i][j] << " ";
				}
			}
			skeletonFileOut << "\n";
		}

		skeletonFileOut << "SubmeshOffset " << "\n";
		auto & e = outSkinnedData.GetSubmeshOffset();
		for(uint32_t i = 0; i < boneSize; ++i)
		{
			skeletonFileOut << e[i] << " ";
		}
		skeletonFileOut << "\n";
	}
}

void FbxLoader::ExportMesh(
	std::vector<Vertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	std::ofstream fileOut(fileName + ".mesh");

	if (outVertexVector.empty() || outIndexVector.empty())
		return;

	if (fileOut)
	{
		uint32_t vertexSize = outVertexVector.size();
		uint32_t indexSize = outIndexVector.size();
		uint32_t materialSize = outMaterial.size();

		fileOut << "VertexSize " << vertexSize << "\n";
		fileOut << "IndexSize " << indexSize << "\n";
		fileOut << "MaterialSize " << materialSize << "\n";

		fileOut << "Material " << "\n";
		for (auto & e : outMaterial)
		{
			fileOut << "Name " << e.Name << "\n";
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

		for (auto& e : outVertexVector)
		{
			fileOut << "Pos " << e.Pos.x << " " << e.Pos.y << " " << e.Pos.z << "\n";
			fileOut << "Normal " << e.Normal.x << " " << e.Normal.y << " " << e.Normal.z << "\n";
			fileOut << "TexC " << e.TexC.x << " " << e.TexC.y << "\n";
			fileOut << "Tangent " << e.Tangent.x << " " << e.Tangent.y << " " << e.Tangent.z << "\n";
			fileOut << "Binormal " << e.Binormal.x << " " << e.Binormal.y << " " << e.Binormal.z << "\n";
		}

		fileOut << "Indices " << "\n";
		for (uint32_t i = 0; i < indexSize / 3; ++i)
		{
			fileOut << outIndexVector[3 * i] << " " << outIndexVector[3 * i + 1] << " " << outIndexVector[3 * i + 2] << "\n";
		}
		std::cout << "Mesh Complit! " << std::endl;
	}

}

void FbxLoader::ExportMesh(
	std::vector<CharacterVertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector,
	std::vector<Material>& outMaterial,
	std::string fileName)
{
	std::ofstream fileOut(fileName + ".cmesh");

	if (outVertexVector.empty() || outIndexVector.empty())
		return;

	if (fileOut)
	{
		uint32_t vertexSize = outVertexVector.size();
		uint32_t indexSize = outIndexVector.size();
		uint32_t materialSize = outMaterial.size();

		fileOut << "VertexSize " << vertexSize << "\n";
		fileOut << "IndexSize " << indexSize << "\n";
		fileOut << "MaterialSize " << materialSize << "\n";

		fileOut << "Material " << "\n";
		for (auto & e : outMaterial)
		{
			fileOut << "Name " << e.Name << "\n";
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

		
		for (auto& e : outVertexVector)
		{
			fileOut << "Pos " << e.Pos.x << " " << e.Pos.y << " " << e.Pos.z << "\n";
			fileOut << "Normal " << e.Normal.x << " " << e.Normal.y << " " << e.Normal.z << "\n";
			fileOut << "TexC " << e.TexC.x << " " << e.TexC.y << "\n";
			fileOut << "Tangent " << e.Tangent.x << " " << e.Tangent.y << " " << e.Tangent.z << "\n";
			fileOut << "Binormal " << e.Binormal.x << " " << e.Binormal.y << " " << e.Binormal.z << "\n";

			fileOut << "BoneWeight " << e.BoneWeights.x << " " << e.BoneWeights.y << " " << e.BoneWeights.z << "\n";
			fileOut << "BoneIndices " << (int)e.BoneIndices[0] << " " << (int)e.BoneIndices[1] << " " << (int)e.BoneIndices[2] << " " << (int)e.BoneIndices[3] << "\n";
		}


		fileOut << "Indices " << "\n";
		for (uint32_t i = 0; i < indexSize / 3; ++i)
		{
			fileOut << outIndexVector[3 * i] << " " << outIndexVector[3 * i + 1] << " " << outIndexVector[3 * i + 2] << "\n";
		}
	}
}

void FbxLoader::clear()
{
	mControlPoints.clear();
	mBoneName.clear();
	mBoneHierarchy.clear();
	mBoneOffsets.clear();
	mAnimations.clear();
}