// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "ModelManager.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/HMeshAsset.h"
#include "VertexLayouts.h"
#include "Hash.h"


extern StaticGeometryManager GStaticGeometryManager;


void ManagedStaticMeshGeometry::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
		std::this_thread::yield();
}

void ManagedStaticMeshGeometry::Unload()
{
	GStaticGeometryManager.DestroyMesh( m_MapKey );
}

void StaticGeometryManager::DestroyMesh( const String& Key )
{
	auto Iter = m_ModelCache.find( Key );
	if (Iter != m_ModelCache.end())
		m_ModelCache.erase( Iter );

}

bool StaticGeometryManager::MeshExists( const String& Name )
{
	auto Iter = m_ModelCache.find( Name );
	return Iter != m_ModelCache.end();
}

void StaticGeometryManager::FlushCache()
{
	m_ModelCache.clear();
}

StaticMeshGeometryRef StaticGeometryManager::LoadHAssetMeshFromFile( const String& FilePath )
{
	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath );
	if (MeshExists( MeshName ))
		return GetStaticMeshByName( MeshName );


	// Read the data file from disk.
	//
	DataBlob Memory = FileSystem::ReadRawData( FilePath.c_str() );
	HE_ASSERT( Memory.IsValid() ); // Failed to load model.

	uint8* DataHead = Memory.GetBufferPointer();

	// Copy the header information.
	const MeshAssetHeader& MeshHeader = *(MeshAssetHeader*)DataHead;
	HE_ASSERT( MeshHeader.Type == AT_Mesh ); // Trying to parse an asset that is not a mesh!
	DataHead += kMeshHeaderSizeInBytes;		// Now pointing to first vertex in the vertex buffer.

	// Get the vertex data.
	const uint32 VertexBufferSize = MeshHeader.NumVerticies * MeshHeader.VertexSizeInBytes;
	uint8* FirstVertex = DataHead;
	DataHead += VertexBufferSize; // Now pointing to first index.

	// Get the index data.
	const uint32 IndexBufferSize = MeshHeader.NumIndices * MeshHeader.IndexSizeInBytes;
	uint8* FirstIndex = DataHead;


	// Create the mesh geometry, register it with the GPU and cache it.
	uint64 NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	ManagedStaticMeshGeometry* pMesh = new ManagedStaticMeshGeometry( MeshName );
	pMesh->SetHashName( NameHash );
	pMesh->Create(
		FirstVertex, MeshHeader.NumVerticies, MeshHeader.VertexSizeInBytes,
		FirstIndex, IndexBufferSize, MeshHeader.NumIndices
	);
	pMesh->SetLoadCompleted( true );
	m_ModelCache[MeshName].reset( pMesh );

	return m_ModelCache[MeshName].get();
}

StaticMeshGeometryRef StaticGeometryManager::RegisterGeometry( const std::string& Name, void* Verticies, uint32 NumVerticies, uint32 VertexSizeInBytes, void* Indices, uint32 IndexDataSizeInBytes, uint32 NumIndices )
{
#if HE_WITH_EDITOR
	HE_ASSERT( MeshExists( Name ) == false ); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	uint64 HashName = StringHash( Name.c_str(), Name.size() );
	ManagedStaticMeshGeometry* pMesh = new ManagedStaticMeshGeometry( Name );
	pMesh->SetHashName( HashName );
	pMesh->Create( Verticies, NumVerticies, VertexSizeInBytes, Indices, IndexDataSizeInBytes, NumIndices );
	pMesh->SetLoadCompleted( true );

	m_ModelCache[Name].reset( pMesh );

	return pMesh;
}

StaticMeshGeometryRef::StaticMeshGeometryRef( const StaticMeshGeometryRef& ref )
	: m_Ref( ref.m_Ref )
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

StaticMeshGeometryRef::StaticMeshGeometryRef( ManagedStaticMeshGeometry* tex )
	: m_Ref( tex )
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

StaticMeshGeometryRef::~StaticMeshGeometryRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

void StaticMeshGeometryRef::operator= ( std::nullptr_t )
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

void StaticMeshGeometryRef::operator= ( StaticMeshGeometryRef& rhs )
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

bool StaticMeshGeometryRef::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

StaticMeshGeometry* StaticMeshGeometryRef::Get()
{
	return (StaticMeshGeometry*)m_Ref;
}

StaticMeshGeometry* StaticMeshGeometryRef::operator->()
{
	HE_ASSERT( m_Ref != nullptr );
	return (StaticMeshGeometry*)m_Ref;
}

