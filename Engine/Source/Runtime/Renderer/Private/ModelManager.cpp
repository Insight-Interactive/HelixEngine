// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "ModelManager.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "VertexLayouts.h"
#include "Hash.h"

//extern "C" {
//#include "libdeflate.c"
//}
//#include "ofbx.h"
//#include "ofbx.cpp"


extern FGeometryManager GGeometryManager;

StaticMeshGeometryRef FGeometryManager::ProcessNode( aiNode* node, const aiScene* scene )
{     
	for (uint32 i = 0; i < node->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		return ProcessMesh( mesh, scene );
	}

	for (UINT i = 0; i < node->mNumChildren; i++) 
	{
		return ProcessNode( node->mChildren[i], scene );
	}

	return nullptr;
}

StaticMeshGeometryRef FGeometryManager::ProcessMesh( aiMesh* mesh, const aiScene* scene )
{
	uint32 TotalVerticies = mesh->mNumVertices;
	uint32 TotalIndices = mesh->mNumFaces * 3;
	uint32 TotalBones = mesh->mNumBones;

	// Data to fill
	std::vector<FVertex3D> Vertices( TotalVerticies );
	std::vector<uint32> Indices( TotalIndices );

	// Walk through each of the mesh's vertices
	for (uint32 i = 0; i < TotalVerticies; i++)
	{
		FVertex3D& vertex = Vertices[i];

		if (mesh->HasPositions())
		{
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;
		}
		
		if (mesh->mTextureCoords[0]) 
		{
			vertex.UV0.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.UV0.y = (float)mesh->mTextureCoords[0][i].y;
		}

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;

			vertex.BiTangent.x = mesh->mBitangents[i].x;
			vertex.BiTangent.y = mesh->mBitangents[i].y;
			vertex.BiTangent.z = mesh->mBitangents[i].z;
		}
	}


	for (uint32 i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];

		for (uint32 j = 0; j < face.mNumIndices; j++)
			Indices.push_back( face.mIndices[j] );
	}


	// Create the mesh geometry, register it with the GPU and cache it.
	HHash NameHash = StringHash( mesh->mName.C_Str(), mesh->mName.length );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( mesh->mName.C_Str() );
	pMesh->SetHashName( NameHash );
	pMesh->Create(
		Vertices.data(), (uint32)Vertices.size(), sizeof(FVertex3D),
		Indices.data(), (uint32)Indices.size() * sizeof( uint32 ), (uint32)Indices.size()
	);
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_StaticModelCacheMutex );
	m_StaticModelCache[mesh->mName.C_Str()].reset( pMesh );
	return m_StaticModelCache[mesh->mName.C_Str()];
}

StaticMeshGeometryRef FGeometryManager::LoadHAssetMeshFromFile( const String& FilePath )
{
	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (StaticMeshExists( MeshName ))
		return GetStaticMeshByName( MeshName );

	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices );
	return ProcessNode( pScene->mRootNode, pScene );


	// CUSTOM HASSET MESH
	//HE_ASSERT( StringHelper::GetFileExtension( FilePath ) == "hasset" ); // Trying to load a file that is not an hasset.
	//// Read the data file from disk.
	////
	//DataBlob Memory = FileSystem::ReadRawData( FilePath.c_str() );
	//HE_ASSERT( Memory.IsValid() ); // Failed to load model.

	//uint8* DataHead = Memory.GetBufferPointer();

	//// Copy the header information.
	//const HMeshAsset::HMeshAssetHeader& MeshHeader = *(HMeshAsset::HMeshAssetHeader*)DataHead;
	//HE_ASSERT( MeshHeader.Type == AT_Mesh ); // Trying to parse an asset that is not a mesh!
	//DataHead += HMeshAsset::kMeshHeaderSizeInBytes;		// Now pointing to first vertex in the vertex buffer.

	//// Get the vertex data.
	//const uint32 VertexBufferSize = MeshHeader.NumVerticies * MeshHeader.VertexSizeInBytes;
	//uint8* FirstVertex = DataHead;
	//DataHead += VertexBufferSize; // Now pointing to first index.

	//// Get the index data.
	//const uint32 IndexBufferSize = MeshHeader.NumIndices * MeshHeader.IndexSizeInBytes;
	//uint8* FirstIndex = DataHead;


	//// Create the mesh geometry, register it with the GPU and cache it.
	//HHash NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	//HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( MeshName );
	//pMesh->SetHashName( NameHash );
	//pMesh->Create(
	//	FirstVertex, MeshHeader.NumVerticies, MeshHeader.VertexSizeInBytes,
	//	FirstIndex, IndexBufferSize, MeshHeader.NumIndices
	//);
	//pMesh->SetLoadCompleted( true );

	//ScopedCriticalSection Guard( m_MapMutex );
	//m_ModelCache[MeshName].reset( pMesh );
	//return m_ModelCache[MeshName];
}

//struct XMFLOAT4X4
//{
//	union
//	{
//		struct
//		{
//			float _11, _12, _13, _14;
//			float _21, _22, _23, _24;
//			float _31, _32, _33, _34;
//			float _41, _42, _43, _44;
//		};
//		float m[4][4];
//	};

//TReal a1, a2, a3, a4;
//TReal b1, b2, b3, b4;
//TReal c1, c2, c3, c4;
//TReal d1, d2, d3, d4;

void ConvertAssimp4x4Matrix( const aiMatrix4x4& Source, FMatrix& Destination )
{
	Destination._11 = Source.a1;
	Destination._12 = Source.a2;
	Destination._13 = Source.a3;
	Destination._14 = Source.a4;

	Destination._21 = Source.b1;
	Destination._22 = Source.b2;
	Destination._23 = Source.b3;
	Destination._24 = Source.b4;

	Destination._31 = Source.c1;
	Destination._32 = Source.c2;
	Destination._33 = Source.c3;
	Destination._34 = Source.c4;

	Destination._41 = Source.d1;
	Destination._42 = Source.d2;
	Destination._43 = Source.d3;
	Destination._44 = Source.d4;
}

void ProcessJoints( std::vector<FJoint>& Joints, const aiNode* pNode, const uint32& ParentIndex, const aiMesh* pMesh )
{
	FJoint& joint = Joints.emplace_back();
	strcpy_s( joint.m_Name, sizeof( joint.m_Name ), pNode->mName.C_Str());
	joint.m_NameHash = StringHash( joint.m_Name );
	joint.m_ParentIndex = ParentIndex;
	ConvertAssimp4x4Matrix( pNode->mTransformation, joint.m_LocalMatrix );
	for (uint32 i = 0; i < pMesh->mNumBones; i++)
	{
		if (pNode->mName == pMesh->mBones[i]->mName)
		{
			ConvertAssimp4x4Matrix( pMesh->mBones[i]->mOffsetMatrix, joint.m_OffsetMatrix );
			break;
		}
	}

	uint32 NewParentIndex = (uint32)Joints.size() - 1u;
	for (uint32 i = 0; i < pNode->mNumChildren; i++)
	{
		ProcessJoints( Joints, pNode->mChildren[i], NewParentIndex, pMesh );
	}
}

uint8 GetJointIndexFromName( const std::vector<FJoint>& Joints, const Char* Name )
{
	HHash NameHash = StringHash( Name );
	for (uint8 i = 0; i < Joints.size(); i++)
	{
		if (Joints[i].m_NameHash == NameHash)
			return i;
	}

	HE_ASSERT( false ); // Joint not found
	return -1;
}

void ParseBoneWeights( const std::vector<FJoint>& Joints, const aiMesh* pMesh, std::vector<FSkinnedVertex3D>& Vertices, const uint32& BaseVertexIndex )
{
	for (uint32 i = 0; i < pMesh->mNumBones; i++)
	{
		const aiBone* pBone = pMesh->mBones[i];
		const uint8 JointIndex = GetJointIndexFromName( Joints, pBone->mName.C_Str() );

		for (uint32 j = 0; j < pBone->mNumWeights; j++)
		{
			const aiVertexWeight& VertWeight = pBone->mWeights[j];
			const uint32 VertexId = BaseVertexIndex + VertWeight.mVertexId;

			for (uint32 k = 0; k < HE_MAX_JOINTS_PER_VERTEX; k++) // Find a free index in the array
			{
				HE_ASSERT( VertexId < Vertices.size() ); // Vertex is out of bounds

				if (Vertices[VertexId].Joints[k] == 0)
				{
					Vertices[VertexId].Joints[k] = JointIndex;
					Vertices[VertexId].Weights[k] = VertWeight.mWeight;
					break;
				}
			}
		}
	}
}

SkeletalMeshGeometryRef FGeometryManager::SK_ParseMeshes( const aiScene* pScene )
{
	HManagedSkeletalMeshGeometry* pSkeletalMesh = new HManagedSkeletalMeshGeometry( pScene->mName.C_Str() );
	pSkeletalMesh->m_Meshes.resize( pScene->mNumMeshes );


	// Data to fill
	uint32 TotalVerts = 0;
	uint32 TotalIndices = 0;
	for (uint32 i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];

		TotalVerts += pMesh->mNumVertices;

		for (uint32 j = 0; j < pMesh->mNumFaces; j++)
		{
			const aiFace& face = pMesh->mFaces[j];
			TotalIndices += face.mNumIndices;
		}
	}

	std::vector<FSkinnedVertex3D> Vertices(TotalVerts);
	std::vector<uint32> Indices(TotalIndices);

	uint32 BaseVertexIndex = 0;
	uint32 IndexGroupOffset = 0;
	uint32 TotalGroupIndicies = 0;
	for (uint32 i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];


		// Walk through each of the mesh's vertices
		for (uint32 j = 0; j < pMesh->mNumVertices; j++)
		{
			FSkinnedVertex3D& Vertex = Vertices[BaseVertexIndex + j];

			if (pMesh->HasPositions())
			{
				Vertex.Position.x = pMesh->mVertices[j].x;
				Vertex.Position.y = pMesh->mVertices[j].y;
				Vertex.Position.z = pMesh->mVertices[j].z;
			}

			if (pMesh->HasTextureCoords( 0 ))
			{
				Vertex.UV0.x = pMesh->mTextureCoords[0][j].x;
				Vertex.UV0.y = pMesh->mTextureCoords[0][j].y;
			}

			if (pMesh->HasNormals())
			{
				Vertex.Normal.x = pMesh->mNormals[j].x;
				Vertex.Normal.y = pMesh->mNormals[j].y;
				Vertex.Normal.z = pMesh->mNormals[j].z;
			}

			if (pMesh->HasTangentsAndBitangents())
			{
				Vertex.Tangent.x = pMesh->mTangents[j].x;
				Vertex.Tangent.y = pMesh->mTangents[j].y;
				Vertex.Tangent.z = pMesh->mTangents[j].z;

				Vertex.BiTangent.x = pMesh->mBitangents[j].x;
				Vertex.BiTangent.y = pMesh->mBitangents[j].y;
				Vertex.BiTangent.z = pMesh->mBitangents[j].z;
			}
		}

		uint32 FaceOffset = 0;
		for (uint32 k = 0; k < pMesh->mNumFaces; k++)
		{
			const aiFace& face = pMesh->mFaces[k];

			for (uint32 l = 0; l < face.mNumIndices; l++)
			{
				Indices[IndexGroupOffset + (FaceOffset + l)] = face.mIndices[l];
				TotalGroupIndicies++;
			}
			FaceOffset += face.mNumIndices;
		}

		if (pMesh->HasBones())
		{
			if (!pSkeletalMesh->Joints.size())
			{
				aiNode* pArmatureRoot = pMesh->mBones[0]->mArmature;
				// TODO: Go through all the nodes in the armature and find the root bone name
				memcpy( &pSkeletalMesh->m_GlobalInverseTransform, &pArmatureRoot->mTransformation, sizeof( FMatrix ) );
				pSkeletalMesh->m_GlobalInverseTransform = pSkeletalMesh->m_GlobalInverseTransform.Invert();

				//aiNode* pNode = pMesh->mBones[0]->mNode;
				//pSkeletalMesh->Joints.reserve( pMesh->mNumBones );

				ProcessJoints( pSkeletalMesh->Joints, pArmatureRoot, R_JOINT_INVALID_INDEX, pMesh);
			}

			ParseBoneWeights( pSkeletalMesh->Joints, pMesh, Vertices, BaseVertexIndex );
		}

		FSkinnedVertex3D* pVertData = Vertices.data() + BaseVertexIndex;
		uint32* pIndexData = Indices.data() + IndexGroupOffset;

		const char* Name = pMesh->mName.C_Str();
		HHash NameHash = StringHash( Name, strlen( Name ) );
		pSkeletalMesh->m_Meshes[i].SetHashName( NameHash );
		pSkeletalMesh->m_Meshes[i].SetName( Name );
		pSkeletalMesh->m_Meshes[i].Create(
			pVertData, pMesh->mNumVertices, sizeof( FSkinnedVertex3D ),
			pIndexData, TotalGroupIndicies * sizeof( uint32 ), TotalGroupIndicies
		);

		BaseVertexIndex += pMesh->mNumVertices; // pMesh->mNumVertices is not an index so we dont need +1 because we wont overwrite the previous vertex
		IndexGroupOffset += TotalGroupIndicies;
		TotalGroupIndicies = 0;
	}
	pSkeletalMesh->m_JointCB.Create( L"Skeleton Joints" );// TODO: this should belong to each unique instance.
	pSkeletalMesh->SetLoadCompleted( true );

	String Name = pScene->mName.C_Str();

	ScopedCriticalSection Guard( m_SkeletalModelCacheMutex );
	m_SkeletalModelCache[Name].reset( pSkeletalMesh );
	return m_SkeletalModelCache[Name];
}

//SkeletalMeshGeometryRef FGeometryManager::SK_ParseMeshes( const aiScene* pScene )
//{
//	HManagedSkeletalMeshGeometry* pSkeletalMesh = new HManagedSkeletalMeshGeometry( pScene->mName.C_Str() );
//	pSkeletalMesh->m_Meshes.resize( pScene->mNumMeshes );
//
//	/*uint32 NumVertices = 0;
//	uint32 NumIndices = 0;
//	for (uint32  i = 0; i < pScene->mNumMeshes; i++)
//	{
//		NumVertices += pScene->mMeshes[i]->mNumVertices;
//		NumIndices += pScene->mMeshes[i]->mNumFaces * 3;
//
//		pSkeletalMesh->m_Meshes[i].m_DrawArgs.NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
//		pSkeletalMesh->m_Meshes[i].m_DrawArgs.BaseVert = NumVertices;
//		pSkeletalMesh->m_Meshes[i].m_DrawArgs.BaseIndex = NumIndices;
//	}
//
//	pSkeletalMesh->m_BoneData.resize( NumVertices );*/
//
//	for (uint32 i = 0; i < pScene->mNumMeshes; i++)
//	{
//		const aiMesh* pMesh = pScene->mMeshes[i];
//
//
//		// Data to fill
//		std::vector<FSkinnedVertex3D> Vertices( pMesh->mNumVertices );
//		std::vector<uint32> Indices/*( pScene->mMeshes[i]->mNumFaces * 3 )*/;
//
//		// Walk through each of the mesh's vertices
//		for (uint32 j = 0; j < pMesh->mNumVertices; j++)
//		{
//			FSkinnedVertex3D& Vertex = Vertices[j];
//
//			if (pMesh->HasPositions())
//			{
//				Vertex.Position.x = pMesh->mVertices[j].x;
//				Vertex.Position.y = pMesh->mVertices[j].y;
//				Vertex.Position.z = pMesh->mVertices[j].z;
//			}
//			
//			if (pMesh->HasTextureCoords( 0 ))
//			{
//				Vertex.UV0.x = pMesh->mTextureCoords[0][j].x;
//				Vertex.UV0.y = pMesh->mTextureCoords[0][j].y;
//			}
//
//			if (pMesh->HasNormals())
//			{
//				Vertex.Normal.x = pMesh->mNormals[j].x;
//				Vertex.Normal.y = pMesh->mNormals[j].y;
//				Vertex.Normal.z = pMesh->mNormals[j].z;
//			}
//
//			if (pMesh->HasTangentsAndBitangents())
//			{
//				Vertex.Tangent.x = pMesh->mTangents[j].x;
//				Vertex.Tangent.y = pMesh->mTangents[j].y;
//				Vertex.Tangent.z = pMesh->mTangents[j].z;
//
//				Vertex.BiTangent.x = pMesh->mBitangents[j].x;
//				Vertex.BiTangent.y = pMesh->mBitangents[j].y;
//				Vertex.BiTangent.z = pMesh->mBitangents[j].z;
//			}
//		}
//
//		if (pMesh->HasBones())
//		{
//			//SK_ParseMeshBones( i, pMesh, *pSkeletalMesh );
//
//			/*FJoint rootJoint;
//			ProcessJoints( rootJoint, pMesh->mBones[0]->mArmature );*/
//			if (pSkeletalMesh->Joints.size() == 0)
//				ProcessJoints( pSkeletalMesh->Joints, pMesh->mBones[0]->mArmature, 0, pMesh );
//
//			for (uint32 i = 0; i < pMesh->mNumBones; i++)
//			{
//				aiBone* pBone = pMesh->mBones[i];
//				for (uint32 j = 0; j < pBone->mNumWeights; j++)
//				{
//					const aiVertexWeight& VertWeight = pBone->mWeights[j];
//
//					for (uint32 k = 0; k < HE_MAX_JOINTS_PER_VERTEX; k++) // Find a free index in the array
//					{
//						if (Vertices[VertWeight.mVertexId].Joints[k] == 0)
//						{
//							Vertices[VertWeight.mVertexId].Joints[k] = GetJointIndexFromName( pSkeletalMesh->Joints, pBone->mName.C_Str() );
//							Vertices[VertWeight.mVertexId].Weights[k] = VertWeight.mWeight;
//							break;
//						}
//					}
//				}
//			}
//		}
//
//
//
//		for (uint32 k = 0; k < pMesh->mNumFaces; k++) 
//		{
//			const aiFace& face = pMesh->mFaces[k];
//
//			for (uint32 l = 0; l < face.mNumIndices; l++)
//				Indices.push_back( face.mIndices[l] );
//		}
//
//
//		const char* Name = pMesh->mName.C_Str();
//		HHash NameHash = StringHash( Name, strlen( Name ) );
//		pSkeletalMesh->m_Meshes[i].SetHashName( NameHash );
//		pSkeletalMesh->m_Meshes[i].SetName( Name );
//		pSkeletalMesh->m_Meshes[i].Create(
//			Vertices.data(), (uint32)Vertices.size(), sizeof( FSkinnedVertex3D ),
//			Indices.data(), (uint32)Indices.size() * sizeof( uint32 ), (uint32)Indices.size()
//		);
//		pSkeletalMesh->SetLoadCompleted( true );
//
//	}
//	pSkeletalMesh->m_JointCB.Create( L"Skeleton Joints" );
//
//	String Name = pScene->mName.C_Str();
//
//	ScopedCriticalSection Guard( m_SkeletalModelCacheMutex );
//	m_SkeletalModelCache[Name].reset( pSkeletalMesh );
//	return m_SkeletalModelCache[Name];
//}

SkeletalMeshGeometryRef FGeometryManager::SK_ParseScene( const aiScene* pScene )
{
	return SK_ParseMeshes( pScene );
}

SkeletalMeshGeometryRef FGeometryManager::LoadSkeletalMeshFromFile( const String& FilePath )
{
	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (SkeletalMeshExists( MeshName ))
		return GetSkeletalMeshByName( MeshName );

	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* pScene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );
	HE_ASSERT( pScene != nullptr );
	return SK_ParseScene( pScene );
}

StaticMeshGeometryRef FGeometryManager::RegisterGeometry( const std::string& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( StaticMeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	HHash HashName = StringHash( Name.c_str(), Name.size() );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( Name );
	pMesh->SetHashName( HashName );
	pMesh->Create( VertexData, NumVerticies, VertexSizeInBytes, IndexData, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_StaticModelCacheMutex );
	m_StaticModelCache[Name].reset( pMesh );
	return m_StaticModelCache[Name];
}
