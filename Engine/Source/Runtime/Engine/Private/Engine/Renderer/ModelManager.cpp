// Copyright 52024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Renderer/ModelManager.h"
#include "Engine/Physics/Physics.h"
#include "AssetRegistry/AssetDatabase.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "Hash.h"

#include "miniz.c"
#include "ofbx.h"
#include "ofbx.cpp"


extern FStaticGeometryManager GStaticGeometryManager;


HStaticMesh FStaticGeometryManager::LoadHAssetMeshFromFile( const String& FilePath )
{
	//HE_ASSERT( StringHelper::GetFileExtension( FilePath ) == "hasset" ); // Trying to load a file that is not an hasset.

	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (MeshExists( MeshName ))
		return GetStaticMeshByName( MeshName );

	// Read the data file from disk.
	//
	DataBlob FileData = FileSystem::ReadRawData( FilePath.c_str() );


	// Process the mesh as an fbx file.
	//
	ofbx::IScene* pScene = ofbx::load(
		(const ofbx::u8*)FileData.GetBufferPointer(),
		(int)FileData.GetDataSize(),
		(ofbx::u64)ofbx::LoadFlags::TRIANGULATE
	);
	if (pScene == NULL)
	{
		HE_ASSERT( false );
		R_LOG( Error, TEXT( "An error occured when importing model! Error code: %s" ), ofbx::getError() );
		return nullptr;
	}

	const ofbx::Mesh& Mesh = *pScene->getMesh( 0 ); // TODO: What about multiple meshes!?
	std::vector<FStaticVertex3D> Verticies( Mesh.getGeometry()->getVertexCount() );
	std::vector<uint32> Indices( Mesh.getGeometry()->getIndexCount() );

	const ofbx::Geometry& Geometry = *Mesh.getGeometry();

	const ofbx::Vec3* RawVerticies = Geometry.getVertices();
	int VertexCount = Geometry.getVertexCount();

	for (int i = 0; i < VertexCount; ++i)
	{
		FStaticVertex3D& Vertex = Verticies[i];

		Vertex.Position.x = (float)RawVerticies[i].x;
		Vertex.Position.y = (float)RawVerticies[i].y;
		Vertex.Position.z = (float)RawVerticies[i].z;

		if (Geometry.getNormals() != nullptr)
		{
			const ofbx::Vec3* normals = Geometry.getNormals();

			Vertex.Normal.x = (float)normals[i].x;
			Vertex.Normal.y = (float)normals[i].y;
			Vertex.Normal.z = (float)normals[i].z;
		}

		if (Geometry.getUVs() != nullptr)
		{
			const ofbx::Vec2* uvs = Geometry.getUVs();
			int count = Geometry.getIndexCount();

			Vertex.UV0.x = (float)uvs[i].x;
			Vertex.UV0.y = (float)uvs[i].y;

			const ofbx::Vec3* tans = Geometry.getTangents();
			if (tans)
			{
				Vertex.Tangent.x = (float)tans[i].x;
				Vertex.Tangent.y = (float)tans[i].y;
				Vertex.Tangent.z = (float)tans[i].z;

				Vertex.BiTangent = Vertex.Normal.Cross( Vertex.Tangent );
			}
			else
			{
				HE_ASSERT( false );// Mesh does not have tangents or bitangents!

				Vertex.UV0 = FVector2( 0.0f, 0.0f );
				Vertex.Tangent = FVector3( 0.0f, 0.0f, 0.0f );
				Vertex.BiTangent = FVector3( 0.0f, 0.0f, 0.0f );
			}
		}
		
	}

	const int* RawFaceIndicies = Geometry.getFaceIndices();
	int IndexCount = Geometry.getIndexCount();
	for (int i = 0; i < IndexCount; ++i)
	{
		int idx = RawFaceIndicies[i];

		// If the index is negative in fbx that means it is the last index of that polygon.
		// So, make it positive and subtract one.
		if (idx < 0)
			idx = -(idx + 1);

		Indices[i] = idx;
	}
	// Create the mesh geometry, register it with the GPU and cache it.
	StringHashValue NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	ManagedStaticMesh* pMesh = new FStaticMesh();
	pMesh->SetName( MeshName );
	pMesh->GetAsset().Create(
		Verticies.data(), (uint32)Verticies.size(),  sizeof( FStaticVertex3D ),
		Indices.data(), (uint32)Indices.size() * sizeof(uint32), (uint32)Indices.size()
	);
	pMesh->SetLoadCompleted( true );

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
	//StringHashValue NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	//HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( MeshName );
	//pMesh->SetHashName( NameHash );
	//pMesh->Create(
	//	FirstVertex, MeshHeader.NumVerticies, MeshHeader.VertexSizeInBytes,
	//	FirstIndex, IndexBufferSize, MeshHeader.NumIndices
	//);
	//pMesh->SetLoadCompleted( true );

	/*auto Iter = m_NewModelCache.find( MeshName );
	if (Iter == m_NewModelCache.end())
	{
		m_NewModelCache[MeshName] = std::make_pair( std::make_unique<ManagedStaticMesh>(pMesh), std::vector<TConstantBuffer<MeshWorldCBData>>{} );
	}*/

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[MeshName].reset( pMesh );
	return m_ModelCache[MeshName].get();
}

void FStaticGeometryManager::LoadGometry( FPath& FilePath, std::vector<FSimpleVertex3D>& outVerticies, uint32& outVertexCount, std::vector<uint32>& outIndices, uint32& outIndexCount )
{
	// Read the data file from disk.
	//
	DataBlob FileData = FileSystem::ReadRawData( FilePath.m_Path );


	// Process the mesh as an fbx file.
	//
	ofbx::IScene* pScene = ofbx::load(
		(const ofbx::u8*)FileData.GetBufferPointer(),
		(int)FileData.GetDataSize(),
		(ofbx::u64)ofbx::LoadFlags::TRIANGULATE
	);
	if (pScene == NULL)
	{
		HE_ASSERT( false );
		R_LOG( Error, TEXT( "An error occured when importing model! Error code: %s" ), ofbx::getError() );
		return;
	}

	int MeshCount = pScene->getMeshCount();
	for (int i = 0; i < MeshCount; i++)
	{
		const ofbx::Mesh& Mesh = *pScene->getMesh( 0 ); // TODO: What about multiple meshes!?
		outVerticies.resize( Mesh.getGeometry()->getVertexCount() );
		outIndices.resize( Mesh.getGeometry()->getIndexCount() );

		const ofbx::Geometry& Geometry = *Mesh.getGeometry();

		const ofbx::Vec3* RawVerticies = Geometry.getVertices();
		outVertexCount = Geometry.getVertexCount();
		for (uint32 j = 0; j < outVertexCount; ++j)
		{
			FSimpleVertex3D& Vertex = outVerticies[j];

			Vertex.Position.x = (float)RawVerticies[j].x;
			Vertex.Position.y = (float)RawVerticies[j].y;
			Vertex.Position.z = (float)RawVerticies[j].z;
		}

		const int* RawFaceIndicies = Geometry.getFaceIndices();
		outIndexCount = Geometry.getIndexCount();
		for (uint32 k = 0; k < outIndexCount; ++k)
		{
			int idx = RawFaceIndicies[k];

			// If the index is negative in fbx that means it is the last index of that polygon.
			// So, make it positive and subtract one.
			if (idx < 0)
				idx = -(idx + 1);

			outIndices[k] = idx;
		}
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
	m_ModelCache[Name].reset( pMesh );
	return m_ModelCache[Name].get();
}

void FStaticGeometryManager::LoadLevelGeo( const String& FilePath, std::vector<FWorldMesh*>& OutWorld )
{
	Assimp::Importer Importer;
	Importer.SetPropertyBool( "AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS", true );
	const aiScene* pAssimpScene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );
	ProcessNode( pAssimpScene->mRootNode, nullptr, pAssimpScene, OutWorld );

	//int MeshCount = pAssimpScene->getMeshCount();
	//OutWorld.resize( MeshCount );
	//for (int i = 0; i < MeshCount; i++)
	//{
	//	OutWorld[i] = new FWorldMesh();
	//	FWorldMesh& NewWorldMesh = *OutWorld[i];

	//	// TODO: Load proper textures
	//	// TODO: Based on texture set physics filter group?
	//	NewWorldMesh.m_Material = FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" );

	//	// Load physics

	//	for (uint32 j = 0; j < VertexCount; ++j)
	//	{
	//		FSimpleVertex3D& Vertex = PhysicsVerticies[j];

	//		Vertex.Position.x = (float)RawVerticies[j].x;
	//		Vertex.Position.y = (float)RawVerticies[j].y;
	//		Vertex.Position.z = (float)RawVerticies[j].z;
	//	}

	//	// Load the level geo and collision
	//	NewWorldMesh.m_Collision.pTriangleData = PhysicsVerticies.data();
	//	NewWorldMesh.m_Collision.TriCount = VertexCount;
	//	NewWorldMesh.m_Collision.VertexSize = sizeof( FSimpleVertex3D );
	//	NewWorldMesh.m_Collision.pIndexData = Indices.data();
	//	NewWorldMesh.m_Collision.IndexCount = IndexCount;
	//	NewWorldMesh.m_Collision.IndexSize = sizeof( uint32 );


	//	Physics::CreateMesh(
	//		Translation,
	//		Rotation,
	//		Scale,
	//		NewWorldMesh.m_Collision,
	//		false,
	//		nullptr,
	//		false,
	//		10.f,
	//		true, FG_WorldGeometry );
	//	NewWorldMesh.m_Collision.SetDebugName( Mesh.name );
	//}

}

HStaticMesh FStaticGeometryManager::ProcessNode( aiNode* node, aiNode* Parent, const aiScene* Scene, std::vector<FWorldMesh*>& OutWorld )
{
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		FWorldMesh* NewMesh = OutWorld.emplace_back(new FWorldMesh() );
		NewMesh->m_Material = FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" );

		aiMesh* Mesh = Scene->mMeshes[node->mMeshes[i]];
		
		aiMatrix4x4 ParentMat = Parent != nullptr ? Parent->mTransformation : aiMatrix4x4();
		aiMatrix4x4 NodeWorldTransformation = ParentMat * node->mTransformation;
		aiVector3t Scaling(1.f, 1.f, 1.f);
		aiVector3t Rotation( 0.f, 0.f, 0.f );
		aiVector3t Position( 0.f, 0.f, 0.f );
		NodeWorldTransformation.Decompose( Scaling, Rotation, Position );
		memcpy( &NewMesh->m_MeshWorldCB->kWorldMat, &NodeWorldTransformation, sizeof( FMatrix ) );
		
		{
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
				FQuat(RotationQuat.x, RotationQuat.y, RotationQuat.z, RotationQuat.w ),
				FVector3( Scaling.x, Scaling.y, Scaling.z ),
				NewMesh->m_Collision,
				false,
				nullptr,
				false,
				10.f,
				true, FG_WorldGeometry );
			NewMesh->m_Collision.SetDebugName( Mesh->mName.C_Str() );
		}

		ProcessMesh( Mesh, Scene, *NewMesh );
	}

	for (uint32 i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode( node->mChildren[i], node, Scene, OutWorld );
	}

	return nullptr;
}

void FStaticGeometryManager::ProcessMesh( aiMesh* mesh, const aiScene* scene, FWorldMesh& OutWorldMesh )
{
	uint32 TotalVerticies = mesh->mNumVertices;
	uint32 TotalIndices = mesh->mNumFaces * 3;

	// Data to fill
	std::vector<FStaticVertex3D> Vertices( TotalVerticies );
	std::vector<uint32> Indices( TotalIndices );

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
			HE_ASSERT( false ); // Mesh does not have tangents or bitangents!

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
	OutWorldMesh.m_Mesh = new FStaticMesh();
	OutWorldMesh.m_Mesh->SetName( mesh->mName.C_Str() );
	OutWorldMesh.m_Mesh->Create(
		Vertices.data(), (uint32)Vertices.size(), sizeof( FStaticVertex3D ),
		Indices.data(), (uint32)Indices.size() * sizeof( uint32 ), (uint32)Indices.size()
	);
	OutWorldMesh.m_Mesh->SetLoadCompleted( true );
}
