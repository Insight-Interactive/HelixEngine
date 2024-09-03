// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Renderer/ModelManager.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "Hash.h"

#include "miniz.c"
#include "ofbx.h"
#include "ofbx.cpp"


extern FStaticGeometryManager GStaticGeometryManager;


void HManagedStaticMeshGeometry::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
		std::this_thread::yield();
}

void HManagedStaticMeshGeometry::Unload()
{
	GStaticGeometryManager.DestroyMesh( m_MapKey );
}

StaticMeshGeometryRef FStaticGeometryManager::LoadHAssetMeshFromFile( const String& FilePath )
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
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( MeshName );
	pMesh->Create(
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

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[MeshName].reset( pMesh );
	return m_ModelCache[MeshName];
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

	const ofbx::Mesh& Mesh = *pScene->getMesh( 0 ); // TODO: What about multiple meshes!?
	outVerticies.resize( Mesh.getGeometry()->getVertexCount() );
	outIndices.resize( Mesh.getGeometry()->getIndexCount() );

	const ofbx::Geometry& Geometry = *Mesh.getGeometry();

	const ofbx::Vec3* RawVerticies = Geometry.getVertices();
	outVertexCount = Geometry.getVertexCount();

	for (uint32 i = 0; i < outVertexCount; ++i)
	{
		FSimpleVertex3D& Vertex = outVerticies[i];

		Vertex.Position.x = (float)RawVerticies[i].x;
		Vertex.Position.y = (float)RawVerticies[i].y;
		Vertex.Position.z = (float)RawVerticies[i].z;
	}

	const int* RawFaceIndicies = Geometry.getFaceIndices();
	outIndexCount = Geometry.getIndexCount();
	for (uint32 i = 0; i < outIndexCount; ++i)
	{
		int idx = RawFaceIndicies[i];

		// If the index is negative in fbx that means it is the last index of that polygon.
		// So, make it positive and subtract one.
		if (idx < 0)
			idx = -(idx + 1);

		outIndices[i] = idx;
	}
}

StaticMeshGeometryRef FStaticGeometryManager::RegisterGeometry( const std::string& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( MeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	StringHashValue HashName = StringHash( Name.c_str(), Name.size() );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( Name );
	pMesh->Create( VertexData, NumVerticies, VertexSizeInBytes, IndexData, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[Name].reset( pMesh );
	return m_ModelCache[Name];
}
