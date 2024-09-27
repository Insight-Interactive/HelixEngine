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
	m_ModelCache[MeshName] =Mesh;
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
