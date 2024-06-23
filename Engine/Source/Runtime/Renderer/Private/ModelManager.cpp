// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "ModelManager.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "AssetRegistry/Asset.h"
#include "VertexLayouts.h"
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

	const ofbx::u8* pData = (const ofbx::u8*)FileData.GetBufferPointer();

	// Process the mesh as an fbx file.
	//
	ofbx::IScene* pScene = ofbx::load(
		pData,
		(int)FileData.GetDataSize(),
		(ofbx::u64)ofbx::LoadFlags::TRIANGULATE
	);
	if (pScene == NULL)
	{
		R_LOG( Error, TEXT( "An error occured when importing model! Error code: %s" ), ofbx::getError() );
		return nullptr;
	}

	const ofbx::Mesh& Mesh = *pScene->getMesh( 0 ); // TODO: What about multiple meshes!?
	std::vector<FVertex3D> Verticies; Verticies.reserve( Mesh.getGeometry()->getVertexCount() );
	std::vector<uint32> Indices; Indices.reserve( Mesh.getGeometry()->getIndexCount() );

	const ofbx::Geometry& Geometry = *(Mesh.getGeometry());

	const ofbx::Vec3* RawVerticies = Geometry.getVertices();
	int VertexCount = Geometry.getVertexCount();

	for (int i = 0; i < VertexCount; ++i)
	{
		FVertex3D Vertex;

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

				FVector3 TempTangent( (float)tans[i].x, (float)tans[i].y, (float)tans[i].z );
				FVector3 Normal( Vertex.Normal.x, Vertex.Normal.y, Vertex.Normal.z );
				FVector3 BiTangent = Normal.Cross( TempTangent );

				Vertex.BiTangent.x = BiTangent.x;
				Vertex.BiTangent.y = BiTangent.y;
				Vertex.BiTangent.z = BiTangent.z;
			}
			else
			{
				HE_ASSERT( false );// Mesh does not have tangents or bitangents!

				Vertex.UV0 = FVector2( 0.0f, 0.0f );
				Vertex.Tangent = FVector3( 0.0f, 0.0f, 0.0f );
				Vertex.BiTangent = FVector3( 0.0f, 0.0f, 0.0f );
			}

			if (Geometry.getColors() != NULL)
			{
				const ofbx::Vec4* Colors = Geometry.getColors();
				Vertex.Color.x = (float)Colors[0].x;
				Vertex.Color.y = (float)Colors[0].y;
				Vertex.Color.z = (float)Colors[0].z;
				Vertex.Color.w = (float)Colors[0].w;
			}
		}

		Verticies.push_back( Vertex );
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

		Indices.push_back( idx );
	}
	// Create the mesh geometry, register it with the GPU and cache it.
	StringHashValue NameHash = StringHash( MeshName.c_str(), MeshName.size() );
	HManagedStaticMeshGeometry* pMesh = new HManagedStaticMeshGeometry( MeshName );
	pMesh->Create(
		Verticies.data(), (uint32)Verticies.size(),  sizeof(FVertex3D),
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
