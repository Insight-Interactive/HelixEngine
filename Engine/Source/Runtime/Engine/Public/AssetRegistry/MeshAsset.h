#pragma once

#include "AssetRegistry/Asset.h"


/////////////////////////
// Mesh Asset Design
// All models are imported and their meshes are extracted
// turned into this custom format. (.fbx/.obj -> .hasset)
// 
// Header:
//		Num Verticies	( 4 Bytes )
//		Vertex Size		( 4 Bytes )
//		Num Indicies	( 4 Bytes )
//		Index Size		( 4 Bytes )
// 
// Mesh Data:
//		Mesh1 Vertex 1
//		... Mesh1 n verticies
//		... Num Verticies * Vertex Size
//		Mesh1 Indices
//		... Mesh1 n indices
//		... Num Indicies * Index Size
//		
/////////////////////////

struct FMeshAssetHeader
{
private:
	friend class FStaticGeometryManager;
	const EAssetType Type = AT_Mesh;
	
public:

	/*
		Number of verticies in the mesh.
	*/
	uint32 NumVerticies;

	/*
		Size of each vertex in bytes.
	*/
	uint32 VertexSizeInBytes;

	/*
		Number of indices this mesh posesses.
	*/
	uint32 NumIndices;

	/*
		Size of each vertex in bytes.
	*/
	uint32 IndexSizeInBytes;
};
static const uint32 kMeshHeaderSizeInBytes = sizeof( FMeshAssetHeader );


class HMeshAsset : public HAsset
{
public:
	HMeshAsset( );
	virtual ~HMeshAsset();

	virtual void Serialize( const char* Filename ) override;
	virtual void DeSerialize( const char* Filename ) override;


protected:

};
