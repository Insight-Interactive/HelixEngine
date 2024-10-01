#include "EnginePCH.h"

#include "Engine/Renderer/Mesh.h"

void FMesh::Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices )
{
	// Create the vertex buffer.
	m_DrawArgs.NumVerts = NumVerticies;
	m_DrawArgs.m_VertexBuffer.Create( L"Vertex Buffer", NumVerticies * VertexSize, VertexSize, pVertexData );

	// Create the index buffer.
	m_DrawArgs.NumIndices = NumIndices;
	m_DrawArgs.m_IndexBuffer.Create( L"Index Buffer", IndexDataSize, pIndexData );
}
