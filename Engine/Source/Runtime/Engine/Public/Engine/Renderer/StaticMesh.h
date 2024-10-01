// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/ManagedAsset.h"

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

#include "Engine/Renderer/Mesh.h"


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

	FMesh& GetMesh() { return m_Mesh; }
	void Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices );

private:
	void SetGuid( const FGUID& NewGUID ) { m_Guid = NewGUID; }

	FMesh m_Mesh;
	FGUID m_Guid;
	// TODO AABB for culling
};

typedef ManagedAsset<FStaticMesh> ManagedStaticMesh;
typedef AssetRef<FStaticMesh> HStaticMesh;
