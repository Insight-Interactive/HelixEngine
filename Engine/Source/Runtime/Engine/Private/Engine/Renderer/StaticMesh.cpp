#include "EnginePCH.h"

#include "Engine/Renderer/StaticMesh.h"

#include "Engine/Renderer/RendererCore.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/VertexBuffer.h"


void FStaticMesh::Create(void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices)
{
	m_Mesh.Create( pVertexData, NumVerticies, VertexSize, pIndexData, IndexDataSize, NumIndices );
}
