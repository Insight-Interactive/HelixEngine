#include "RendererPCH.h"
#if R_WITH_D3D12

#include "GeometryBufferManager.h"
#include "CommandManager.h"


VertexBufferUID FGeometryBufferManager::AllocateVertexBuffer()
{
	ScopedCriticalSection Guard( m_VertexBufferGuard );

	VertexBufferUID NewUID = SNextVertexBufferID++;

	auto InsertResult = m_VertexBufferLUT.try_emplace( NewUID );
	HE_ASSERT( InsertResult.second == true ); // Trying to create a vertex buffer with an already existing ID! This is not allowed.
	InsertResult.first->second.SetUID( NewUID );

	return NewUID;
}

IndexBufferUID FGeometryBufferManager::AllocateIndexBuffer()
{
	ScopedCriticalSection Guard( m_IndexBufferGuard );

	IndexBufferUID NewUID = SNextIndexBufferID++;
	auto InsertResult = m_IndexBufferLUT.try_emplace( NewUID );
	HE_ASSERT( InsertResult.second == true ); // Trying to create a index buffer with an already existing ID! This is not allowed.
	InsertResult.first->second.SetUID( NewUID );

	return NewUID;
}

void FGeometryBufferManager::DeAllocateVertexBuffer( VertexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_VERTEX_BUFFER_HANDLE );

	// Flush the currently executing gpu commands so we dont destroy resoures while they're in flight.
	GCommandManager.IdleGpu();

	m_VertexBufferLUT.erase( UID );
	UID = HE_INVALID_VERTEX_BUFFER_HANDLE;
}

void FGeometryBufferManager::DeAllocateIndexBuffer( IndexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_INDEX_BUFFER_HANDLE );
	
	// Flush the currently executing gpu commands so we dont destroy resoures while they're in flight.
	GCommandManager.IdleGpu();

	m_IndexBufferLUT.erase( UID );
	UID = HE_INVALID_INDEX_BUFFER_HANDLE;
}

#endif // R_WITH_D3D12
