// Copyright 52024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Renderer/ModelManager.h"
#include "Engine/Physics/Physics.h"
#include "AssetRegistry/AssetDatabase.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "Hash.h"

extern FStaticGeometryManager GStaticGeometryManager;
extern FSkeletalGeometryManager GSkeletalGeometryManager;


HStaticMesh FStaticGeometryManager::LoadHAssetMeshFromFile( const String& FilePath )
{
	//HE_ASSERT( StringHelper::GetFileExtension( FilePath ) == "hasset" ); // Trying to load a file that is not an hasset.

	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (MeshExists( MeshName ))
		return GetStaticMeshByName( MeshName );


	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* Scene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );

	HStaticMesh Mesh;
	aiNode* Node = Scene->mRootNode->mChildren[0];
	aiMesh* aiMesh = Scene->mMeshes[Node->mMeshes[0]];
	ProcessMesh( aiMesh, Scene, Mesh );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[MeshName] = Mesh;
	return m_ModelCache[MeshName];
}

void FStaticGeometryManager::LoadBasicGometry( FPath& FilePath, std::vector<FSimpleVertex3D>& outVerticies, uint32& outVertexCount, std::vector<uint32>& outIndices, uint32& outIndexCount )
{
	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* Scene = Importer.ReadFile( FilePath.m_Path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );

	aiNode* Node = Scene->mRootNode->mChildren[0];
	aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[0]];

	// Data to fill
	outVertexCount = Mesh->mNumVertices;
	outIndexCount = Mesh->mNumFaces * 3;
	outVerticies.resize( outVertexCount );
	//outIndices.resize( outIndexCount );


	// Walk through each of the mesh's vertices
	for (uint32 i = 0; i < outVertexCount; i++)
	{
		FSimpleVertex3D& Vertex = outVerticies[i];

		if (Mesh->HasPositions())
		{
			Vertex.Position.x = Mesh->mVertices[i].x;
			Vertex.Position.y = Mesh->mVertices[i].y;
			Vertex.Position.z = Mesh->mVertices[i].z;
		}
	}


	for (uint32 i = 0; i < Mesh->mNumFaces; i++) {
		const aiFace& face = Mesh->mFaces[i];

		for (uint32 j = 0; j < face.mNumIndices; j++)
			outIndices.push_back( face.mIndices[j] );
	}
}

HStaticMesh FStaticGeometryManager::RegisterGeometry( const String& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( MeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	StringHashValue HashName = StringHash( Name.c_str(), Name.size() );
	ManagedStaticMesh* pMesh = new FStaticMesh();
	pMesh->SetName( Name );
	pMesh->GetAsset().Create( VertexData, NumVerticies, VertexSizeInBytes, IndexData, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[Name] = pMesh;
	return m_ModelCache[Name];
}

HStaticMesh FStaticGeometryManager::RegisterAllwaysLoadedGeometry( const String& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( MeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	StringHashValue HashName = StringHash( Name.c_str(), Name.size() );
	ManagedStaticMesh* pMesh = new FStaticMesh();
	pMesh->SetName( Name );
	pMesh->GetAsset().Create( VertexData, NumVerticies, VertexSizeInBytes, IndexData, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_AllwaysLoadedMapMutex );
	m_ModelCacheAllwysLoaded[Name] = pMesh;
	return m_ModelCacheAllwysLoaded[Name];
}

void FStaticGeometryManager::LoadLevelGeo( const String& FilePath, std::vector<FWorldMesh*>& OutWorld )
{
	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* pAssimpScene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );
	ProcessNode( pAssimpScene->mRootNode, nullptr, pAssimpScene, OutWorld );
}

HStaticMesh FStaticGeometryManager::ProcessNode( aiNode* node, aiNode* Parent, const aiScene* Scene, std::vector<FWorldMesh*>& OutWorld )
{
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		FWorldMesh* NewMesh = OutWorld.emplace_back(new FWorldMesh() );
		NewMesh->m_Material = FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" );

		aiMesh* Mesh = Scene->mMeshes[node->mMeshes[i]];
		
		// Load Collision
		aiMatrix4x4 ParentMat = Parent != nullptr ? Parent->mTransformation : aiMatrix4x4();
		aiMatrix4x4 NodeWorldTransformation = ParentMat * node->mTransformation;
		aiVector3t Scaling( 1.f, 1.f, 1.f );
		aiVector3t Rotation( 0.f, 0.f, 0.f );
		aiVector3t Position( 0.f, 0.f, 0.f );
		NodeWorldTransformation.Decompose( Scaling, Rotation, Position );
		memcpy( &NewMesh->m_MeshWorldCB->kWorldMat, &NodeWorldTransformation, sizeof( FMatrix ) );

		uint32 TotalVerticies = Mesh->mNumVertices;
		uint32 TotalIndices = Mesh->mNumFaces * 3;

		// Data to fill
		std::vector<FSimpleVertex3D> Vertices( TotalVerticies );
		std::vector<uint32> Indices;

		// Walk through each of the mesh's vertices
		for (uint32 i = 0; i < TotalVerticies; i++)
		{
			FSimpleVertex3D& Vertex = Vertices[i];

			if (Mesh->HasPositions())
			{
				Vertex.Position.x = Mesh->mVertices[i].x;
				Vertex.Position.y = Mesh->mVertices[i].y;
				Vertex.Position.z = Mesh->mVertices[i].z;
			}
		}

		for (uint32 i = 0; i < Mesh->mNumFaces; i++) {
			const aiFace& face = Mesh->mFaces[i];

			for (uint32 j = 0; j < face.mNumIndices; j++)
				Indices.push_back( face.mIndices[j] );
		}

		// Load the level collision.
		NewMesh->m_Collision.pTriangleData = Vertices.data();
		NewMesh->m_Collision.TriCount = TotalVerticies;
		NewMesh->m_Collision.VertexSize = sizeof( FSimpleVertex3D );
		NewMesh->m_Collision.pIndexData = Indices.data();
		NewMesh->m_Collision.IndexCount = TotalIndices;
		NewMesh->m_Collision.IndexSize = sizeof( uint32 );

		aiQuaternion RotationQuat( Rotation.y, Rotation.z, Rotation.x );
		Physics::CreateMesh(
			FVector3( Position.x, Position.y, Position.z ),
			FQuat( RotationQuat.x, RotationQuat.y, RotationQuat.z, RotationQuat.w ),
			FVector3( Scaling.x, Scaling.y, Scaling.z ),
			NewMesh->m_Collision,
			false,
			nullptr,
			false,
			10.f,
			true, FG_WorldGeometry );
		NewMesh->m_Collision.SetDebugName( Mesh->mName.C_Str() );

		// Load the mesh
		ProcessMesh( Mesh, Scene, NewMesh->m_Mesh );
	}

	for (uint32 i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode( node->mChildren[i], node, Scene, OutWorld );
	}

	return nullptr;
}

void FStaticGeometryManager::ProcessMesh( aiMesh* mesh, const aiScene* scene, HStaticMesh& OutMesh )
{
	uint32 TotalVerticies = mesh->mNumVertices;
	uint32 TotalIndices = mesh->mNumFaces * 3;

	// Data to fill
	std::vector<FStaticVertex3D> Vertices( TotalVerticies );
	std::vector<uint32> Indices;

	// Walk through each of the mesh's vertices
	for (uint32 i = 0; i < TotalVerticies; i++)
	{
		FStaticVertex3D& Vertex = Vertices[i];

		if (mesh->HasPositions())
		{
			Vertex.Position.x = mesh->mVertices[i].x;
			Vertex.Position.y = mesh->mVertices[i].y;
			Vertex.Position.z = mesh->mVertices[i].z;
		}
		
		if (mesh->HasTextureCoords( 0 ))
		{
			Vertex.UV0.x = (float)mesh->mTextureCoords[0][i].x;
			Vertex.UV0.y = (float)mesh->mTextureCoords[0][i].y;
		}

		if (mesh->HasNormals())
		{
			Vertex.Normal.x = mesh->mNormals[i].x;
			Vertex.Normal.y = mesh->mNormals[i].y;
			Vertex.Normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			Vertex.Tangent.x = mesh->mTangents[i].x;
			Vertex.Tangent.y = mesh->mTangents[i].y;
			Vertex.Tangent.z = mesh->mTangents[i].z;

			Vertex.BiTangent.x = mesh->mBitangents[i].x;
			Vertex.BiTangent.y = mesh->mBitangents[i].y;
			Vertex.BiTangent.z = mesh->mBitangents[i].z;
		}
		else
		{
			//HE_ASSERT( false ); // Mesh does not have tangents or bitangents!

			Vertex.UV0 = FVector2( 0.0f, 0.0f );
			Vertex.Tangent = FVector3( 0.0f, 0.0f, 0.0f );
			Vertex.BiTangent = FVector3( 0.0f, 0.0f, 0.0f );
		}
	}


	for (uint32 i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];

		for (uint32 j = 0; j < face.mNumIndices; j++)
			Indices.push_back( face.mIndices[j] );
	}


	// Create the mesh geometry, register it with the GPU and cache it.
	OutMesh = new FStaticMesh();
	OutMesh->SetName( mesh->mName.C_Str() );
	OutMesh->Create(
		Vertices.data(), (uint32)Vertices.size(), sizeof( FStaticVertex3D ),
		Indices.data(), (uint32)Indices.size() * sizeof( uint32 ), (uint32)Indices.size()
	);
	OutMesh->SetLoadCompleted( true );
}

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
	strcpy_s( joint.m_Name, sizeof( joint.m_Name ), pNode->mName.C_Str() );
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
	StringHashValue NameHash = StringHash( Name );
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
			const uint32& VertexId = BaseVertexIndex + VertWeight.mVertexId;

			HE_ASSERT( VertexId < Vertices.size() ); // Vertex is out of bounds

			for (uint32 k = 0; k < R_MAX_JOINTS_PER_VERTEX; k++) // Find a free index in the array
			{
				if (Vertices[VertexId].Joints[k] == 0u)
				{
					Vertices[VertexId].Joints[k] = JointIndex;
					Vertices[VertexId].Weights[k] = VertWeight.mWeight;
					break;
				}
			}
		}
	}
}

HSkeletalMesh FSkeletalGeometryManager::LoadSkeletalMesh( FPath& Path )
{
	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( Path.m_Path );
	if (MeshExists( MeshName ))
		return GetSkeletalMeshByName( MeshName );


	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* Scene = Importer.ReadFile( Path.m_Path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );
	if (Scene == nullptr)
	{
		HE_LOG( Error, "Failed to Load assimp model! Assimp: %s", Importer.GetErrorString() );
		HE_ASSERT( false );
	}

	HSkeletalMesh Mesh = SK_ParseMeshes( Scene );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[MeshName] = Mesh;
	return m_ModelCache[MeshName];
}


HSkeletalMesh FSkeletalGeometryManager::SK_ParseMeshes( const aiScene* pScene )
{
	HSkeletalMesh Mesh = new FSkeletalMesh();
	Mesh->m_Meshes.resize( pScene->mNumMeshes );


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

	std::vector<FSkinnedVertex3D> Vertices( TotalVerts );
	std::vector<uint32> Indices( TotalIndices );

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

		// Walk the indices
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

		// Parse the bones
		if (pMesh->HasBones())
		{
			if (!Mesh->Joints.size())
			{
				aiNode* pArmatureRoot = pMesh->mBones[0]->mNode;

				//for (uint32 z = 0; z < pMesh->mNumBones; z++)
				//{
				//	aiBone* pBone = pMesh->mBones[z];
				//	//uint32 ParentIndex = GetParentIndexFromArmature( pArmatureRoot, pBone->mName.C_Str(), R_JOINT_INVALID_INDEX );
				//	//R_LOG( Log, TEXT( "Bone: %s | ParentIndex: %i" ), CharToTChar( pBone->mName.C_Str() ), ParentIndex );
				//	R_LOG( Log, TEXT( "Bone: %s | NodeName: %s" ), CharToTChar( pBone->mName.C_Str() ), CharToTChar( pBone->mNode->mName.C_Str() ) );
				//}

				//memcpy( &pSkeletalMesh->m_GlobalInverseTransform, &pArmatureRoot->mTransformation, sizeof( FMatrix ) );
				//ConvertAssimp4x4Matrix( pArmatureRoot->mTransformation, pSkeletalMesh->m_GlobalInverseTransform );
				//pSkeletalMesh->m_GlobalInverseTransform = pSkeletalMesh->m_GlobalInverseTransform.Invert();

				//aiNode* pNode = pMesh->mBones[0]->mNode;
				//pSkeletalMesh->Joints.reserve( pMesh->mNumBones );

				ProcessJoints( Mesh->Joints, pArmatureRoot, R_JOINT_INVALID_INDEX, pMesh );
				HE_ASSERT( Mesh->Joints.size() > 0 );
				Mesh->m_GlobalInverseTransform = Mesh->Joints[0].m_LocalMatrix.Invert();
			}

			ParseBoneWeights( Mesh->Joints, pMesh, Vertices, BaseVertexIndex );
		}

		// Assemble and create the mesh
		FSkinnedVertex3D* pVertData = Vertices.data() + BaseVertexIndex;
		uint32* pIndexData = Indices.data() + IndexGroupOffset;

		const char* Name = pMesh->mName.C_Str();
		StringHashValue NameHash = StringHash( Name, strlen( Name ) );
		Mesh->m_Meshes[i].Create(
			pVertData, pMesh->mNumVertices, sizeof( FSkinnedVertex3D ),
			pIndexData, TotalGroupIndicies * sizeof( uint32 ), TotalGroupIndicies
		);

		BaseVertexIndex += pMesh->mNumVertices; // pMesh->mNumVertices is not an index so we dont need +1 because we wont overwrite the previous vertex
		IndexGroupOffset += TotalGroupIndicies;
		TotalGroupIndicies = 0;
	}
	Mesh->SetName( pScene->mName.C_Str() );
	Mesh->SetLoadCompleted( true );

	return Mesh;
}

HSkeletalMesh FSkeletalGeometryManager::SK_ParseScene( const aiScene* pScene )
{
	return SK_ParseMeshes( pScene );
}