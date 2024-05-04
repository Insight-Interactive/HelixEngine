// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GUID.h"


enum EAssetType
{
	AT_Invalid	= 0,
	AT_Mesh		= 1,
	AT_Texture	= 2,
	AT_Material = 3,
	AT_Shader	= 4,
	AT_Audio	= 5,
	AT_Actor	= 6,
};

class HAsset
{
public:
	const FGUID& GetGuid()	const { return m_Guid; }
	EAssetType GetType()	const { return m_Type; }

protected:
	HAsset( const EAssetType& Type, const FGUID& Guid )
		: m_Type( Type )
		, m_Guid( Guid )
	{
	}
	virtual ~HAsset()
	{
	}

	void SetGuid( const FGUID& NewGuid ) { m_Guid = NewGuid; }

protected:
	EAssetType m_Type;
	FGUID m_Guid;

};

class HActorAsset : public HAsset
{
	friend class AActor;
public:
	HActorAsset( const FGUID& Guid )
		: HAsset( AT_Actor, Guid ) {}

};

class HMeshAsset : public HAsset
{
public:
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
	struct HMeshAssetHeader
	{
	private:
		friend class FGeometryManager;
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
	static const uint32 kMeshHeaderSizeInBytes = sizeof( HMeshAssetHeader );

public:
	HMeshAsset( const FGUID& Guid )
		: HAsset( AT_Mesh, Guid ) {}

};

class HTextureAsset : public HAsset
{
public:
	HTextureAsset( const FGUID& Guid )
		: HAsset( AT_Texture, Guid ) {}
	
};

class HMaterialAsset : public HAsset
{
public:
	HMaterialAsset( const FGUID& Guid )
		: HAsset( AT_Material, Guid ) {}
	
private:
	
};

class HShaderAsset : public HAsset
{
public:
	HShaderAsset( const FGUID& Guid )
		: HAsset( AT_Shader, Guid ) {}
		
};

class HAudioAsset : public HAsset
{
public:
	HAudioAsset( const FGUID& Guid )
		: HAsset( AT_Audio, Guid ) {}

};
