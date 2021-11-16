// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "ModelManager.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "VertexLayouts.h"
#include "Hash.h"


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
	HE_ASSERT( StringHelper::GetFileExtension( FilePath ) == "hasset" ); // Trying to load a file that is not an hasset.

	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (MeshExists( MeshName ))
		return GetStaticMeshByName( MeshName );

	// Read the data file from disk.
	//
	DataBlob Memory = FileSystem::ReadRawData( FilePath.c_str() );
	HE_ASSERT( Memory.IsValid() ); // Failed to load model.

	uint8* DataHead = Memory.GetBufferPointer();

	// Copy the header information.
	const HMeshAsset::HMeshAssetHeader& MeshHeader = *(HMeshAsset::HMeshAssetHeader*)DataHead;
	HE_ASSERT( MeshHeader.Type == AT_Mesh ); // Trying to parse an asset that is not a mesh!
	DataHead += HMeshAsset::kMeshHeaderSizeInBytes;		// Now pointing to first vertex in the vertex buffer.

	// Get the vertex data.
	const uint32 VertexBufferSize = MeshHeader.NumVerticies * MeshHeader.VertexSizeInBytes;
	uint8* FirstVertex = DataHead;
	DataHead += VertexBufferSize; // Now pointing to first index.

	// Get the index data.
	const uint32 IndexBufferSize = MeshHeader.NumIndices * MeshHeader.IndexSizeInBytes;
	uint8* FirstIndex = DataHead;


	// Create the mesh geometry, register it with the GPU and cache it.
	uint64 NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( MeshName );
	pMesh->SetHashName( NameHash );
	pMesh->Create(
		FirstVertex, MeshHeader.NumVerticies, MeshHeader.VertexSizeInBytes,
		FirstIndex, IndexBufferSize, MeshHeader.NumIndices
	);
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[MeshName].reset( pMesh );
	return m_ModelCache[MeshName];
}

StaticMeshGeometryRef FStaticGeometryManager::RegisterGeometry( const std::string& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( MeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	uint64 HashName = StringHash( Name.c_str(), Name.size() );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( Name );
	pMesh->SetHashName( HashName );
	pMesh->Create( VertexData, NumVerticies, VertexSizeInBytes, IndexData, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ModelCache[Name].reset( pMesh );
	return m_ModelCache[Name];
}
