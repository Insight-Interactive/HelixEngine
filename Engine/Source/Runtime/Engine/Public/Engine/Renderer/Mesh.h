// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

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
class RENDER_API FMesh
{
	friend class FStaticGeometryManager;
	friend class FAssetDatabase;
public:
	FMesh()
	{
	}
	virtual ~FMesh()
	{
	}

	//
	// Getters and Setters
	//
	FVertexBuffer& GetVertexBuffer();
	FIndexBuffer& GetIndexBuffer();
	uint32 GetNumVerticies() const;
	uint32 GetNumIndices() const;
	FGUID GetGuid() const;

	void Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices );

private:

	void SetGuid( const FGUID& NewUID );


protected:
	FDrawArgs m_DrawArgs;

	FGUID m_Guid;
	// TODO AABB for culling
};


//
// Inline function definitions
//


inline FVertexBuffer& FMesh::GetVertexBuffer()
{
	return m_DrawArgs.m_VertexBuffer;
}

inline FIndexBuffer& FMesh::GetIndexBuffer()
{
	return m_DrawArgs.m_IndexBuffer;
}

inline uint32 FMesh::GetNumVerticies() const
{
	return m_DrawArgs.NumVerts;
}

inline uint32 FMesh::GetNumIndices() const
{
	return m_DrawArgs.NumIndices;
}

inline FGUID FMesh::GetGuid() const
{
	return m_Guid;
}

inline void FMesh::SetGuid( const FGUID& NewGUID )
{
	m_Guid = NewGUID;
}
