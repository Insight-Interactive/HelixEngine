#include "RendererPCH.h"

#include "GeometryBufferManagerD3D12.h"
#include "ICommandManager.h"


VertexBufferUID GeometryBufferManagerD3D12::AllocateVertexBuffer()
{
	VertexBufferUID NewUID = s_NextVertexBufferID++;

	auto InsertResult = m_VertexBufferLUT.try_emplace(NewUID, VertexBufferD3D12{});
	HE_ASSERT(InsertResult.second == true); // Trying to create a vertex buffer with an already existing ID! This is not allowed.

	m_VertexBufferLUT[NewUID].SetUID(NewUID);

	return NewUID;
}

IndexBufferUID GeometryBufferManagerD3D12::AllocateIndexBuffer()
{
	IndexBufferUID NewUID = s_NextIndexBufferID++;
	auto InsertResult = m_IndexBufferLUT.try_emplace(NewUID, IndexBufferD3D12{});
	HE_ASSERT(InsertResult.second == true); // Trying to create a index buffer with an already existing ID! This is not allowed.

	m_IndexBufferLUT[NewUID].SetUID(NewUID);

	return NewUID;
}

void GeometryBufferManagerD3D12::DeAllocateVertexBuffer(VertexBufferUID& UID)
{
	// Flush the currently executing gpu commands so we dont destroy resoures while they're in flight.
	GCommandManager->IdleGPU();

	HE_ASSERT(UID != HE_INVALID_VERTEX_BUFFER_HANDLE);
	m_VertexBufferLUT.erase(UID);
}

void GeometryBufferManagerD3D12::DeAllocateIndexBuffer(IndexBufferUID& UID)
{
	// Flush the currently executing gpu commands so we dont destroy resoures while they're in flight.
	GCommandManager->IdleGPU();

	HE_ASSERT(UID != HE_INVALID_INDEX_BUFFER_HANDLE);
	m_IndexBufferLUT.erase(UID);
}

