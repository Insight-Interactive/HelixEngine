#include "RendererPCH.h"

#include "MeshGeometry.h"



void FMeshGeometry::Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices )
{
	// Create the vertex buffer.
	m_DrawArgs.NumVerts = NumVerticies;

	// Init vertex buffer.
	HE_ASSERT( m_DrawArgs.VertexBufferHandle != HE_INVALID_VERTEX_BUFFER_HANDLE ); // Vertex buffer was not registered properly with geometry buffer manager.
	FVertexBuffer& Buffer = GGeometryBufferManager.GetVertexBufferByUID( m_DrawArgs.VertexBufferHandle );
	Buffer.Create( L"Vertex Buffer", NumVerticies * VertexSize, VertexSize, pVertexData );

	// Create the index buffer.
	m_DrawArgs.NumIndices = NumIndices;

	// Init index buffer.
	HE_ASSERT( m_DrawArgs.IndexBufferHandle != HE_INVALID_INDEX_BUFFER_HANDLE ); // Index buffer was not registered properly with geometry buffer manager.
	FIndexBuffer& IndexBuffer = GGeometryBufferManager.GetIndexBufferByUID( m_DrawArgs.IndexBufferHandle );
	IndexBuffer.Create( L"Index Buffer", IndexDataSize, pIndexData );
}