// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/ManagedAsset.h"

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"
#include "GUID.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct FDrawArgs
{
	FDrawArgs()
		: NumVerts( 0 )
		, NumIndices( 0 )
	{
	}
	~FDrawArgs()
	{
		NumVerts = -1;
		NumIndices = -1;
	}
	uint32 NumVerts;
	uint32 NumIndices;
	FVertexBuffer m_VertexBuffer;
	FIndexBuffer m_IndexBuffer;
};

/*
	Base class for all static geometry that exists in the world. That is, a peice of 
	geometry with static unskinned polygons.
*/
class FStaticMesh : public ManagedAsset<FStaticMesh>
{
	friend class FStaticGeometryManager;
	friend class FAssetDatabase;
public:
	FStaticMesh()
	{
	}
	virtual ~FStaticMesh()
	{
	}

	//
	// Getters and Setters
	//
	FVertexBuffer& GetVertexBuffer() { return m_DrawArgs.m_VertexBuffer; }
	FIndexBuffer& GetIndexBuffer() { return m_DrawArgs.m_IndexBuffer; }
	uint32 GetNumVerticies() const { return m_DrawArgs.NumVerts; }
	uint32 GetNumIndices() const { return m_DrawArgs.NumIndices; }
	FGUID GetGuid() const { return m_Guid; }

	void Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices );

private:
	void SetGuid( const FGUID& NewGUID ) { m_Guid = NewGUID; }

	FDrawArgs m_DrawArgs;
	FGUID m_Guid;
	// TODO AABB for culling
};

typedef ManagedAsset<FStaticMesh> ManagedStaticMesh;
typedef AssetRef<FStaticMesh> HStaticMesh;
