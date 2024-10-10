#include "EnginePCH.h"
#if R_WITH_D3D12

#include "Engine/Renderer/CommandManager.h"


// ---------------------
// Command Manager
// ---------------------

void FCommandManager::Initialize( FRenderDevice& RenderDevice )
{
	m_pDeviceRef = &RenderDevice;
	HE_ASSERT( m_pDeviceRef != NULL );
	
	// Create the Command Queues
	//
	m_GraphicsQueue.InitializeAPI( RenderDevice );
	m_ComputeQueue.InitializeAPI( RenderDevice );
	// TODO: Create command queue
}

void FCommandManager::UnInitialize()
{
	m_pDeviceRef = NULL;
}

void FCommandManager::CreateNewCommandContext( const ECommandListType& Type, FCommandContext& Context, void** pCommandAllocator )
{
	HE_ASSERT( pCommandAllocator != nullptr );

	ID3D12Device* pID3D12Device = (ID3D12Device*)m_pDeviceRef->GetNativeDevice();
	switch (Type)
	{
	case ECommandListType::CLT_Direct:
		(*pCommandAllocator) = m_GraphicsQueue.RequestAllocator();
		break;
	case ECommandListType::CLT_Compute:
		// TODO
		break;
	default:
		R_LOG( Error, "Unrecognized command list type provided while creating a new command context!");
		HE_ASSERT( false );
		break;
	}

	ID3D12GraphicsCommandList** pD3D12CmdList = RCast<ID3D12GraphicsCommandList**>( Context.GetNativeContextAddress() );

	HRESULT hr = pID3D12Device->CreateCommandList( 0, (D3D12_COMMAND_LIST_TYPE)Type, RCast<ID3D12CommandAllocator*>( *pCommandAllocator ), NULL, IID_PPV_ARGS( pD3D12CmdList ) );
	ThrowIfFailedMsg( hr, "Failed to create command list!" );
}

void FCommandManager::WaitForFence( uint64 FenceValue )
{
	FCommandQueue& Queue = GCommandManager.GetQueue( (ECommandListType)(FenceValue >> 56) );
	Queue.WaitForFence( FenceValue );
}



// -----------------------
//  Command Queue
// -----------------------


FCommandQueue::FCommandQueue( const ECommandListType& Type )
	: m_Type( Type )
	, m_pID3D12CommandQueue( NULL )
	, m_D3D12AllocatorPool( (D3D12_COMMAND_LIST_TYPE)Type )
	, m_pFence( NULL )
	, m_NextFenceValue( 0u )
	, m_LastCompletedFenceValue( 0u )
	, m_FenceEventHandle( NULL )
{
}

uint64 FCommandQueue::ExecuteCommandList( void* pNativeCommandList )
{
	m_FenceMutex.Enter();

	ID3D12CommandList* pCommandList = (ID3D12CommandList*)pNativeCommandList;
	
	HRESULT hr = ((ID3D12GraphicsCommandList*)pCommandList)->Close();
	ThrowIfFailedMsg( hr, "Failed to close graphics command list!" );

	m_pID3D12CommandQueue->ExecuteCommandLists( 1, &pCommandList );
	m_pID3D12CommandQueue->Signal( m_pFence, m_NextFenceValue );

	m_FenceMutex.Exit();
	return m_NextFenceValue++;
}

void FCommandQueue::WaitForFence( uint64 FenceValue )
{
	if (IsFenceCompleted( FenceValue ))
		return;

	{
		ScopedCriticalSection Guard( m_EventMutex );

		m_pFence->SetEventOnCompletion( FenceValue, m_FenceEventHandle );
		WaitForSingleObject( m_FenceEventHandle, INFINITE );
		m_LastCompletedFenceValue = FenceValue;
	}
}

uint64 FCommandQueue::IncrementFence()
{
	m_FenceMutex.Enter();
	m_pID3D12CommandQueue->Signal( m_pFence, m_NextFenceValue );
	m_FenceMutex.Exit();
	return m_NextFenceValue++;
}

bool FCommandQueue::IsFenceCompleted( uint64 FenceValue )
{
	if (FenceValue > m_LastCompletedFenceValue)
		m_LastCompletedFenceValue = HE_MAX( m_LastCompletedFenceValue, m_pFence->GetCompletedValue() );

	return FenceValue <= m_LastCompletedFenceValue;
}

FCommandQueue::~FCommandQueue()
{
	CloseHandle( m_FenceEventHandle );

	HE_COM_SAFE_RELEASE( m_pFence );
	HE_COM_SAFE_RELEASE( m_pID3D12CommandQueue );
}

void FCommandQueue::InitializeAPI( FRenderDevice& RenderDevice )
{
	m_pRenderDeviceRef = &RenderDevice;

	m_D3D12AllocatorPool.Initialize( RenderDevice );
	CreateD3D12Queue();
	CreateSyncObjects();
}

void FCommandQueue::CreateD3D12Queue()
{
	ID3D12Device* pD3D12Device = (ID3D12Device*)m_pRenderDeviceRef->GetNativeDevice();

	D3D12_COMMAND_QUEUE_DESC Desc;
	ZeroMemory( &Desc, sizeof( D3D12_COMMAND_QUEUE_DESC ) );
	Desc.Type = (D3D12_COMMAND_LIST_TYPE)m_Type;
	Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	HRESULT hr = pD3D12Device->CreateCommandQueue( &Desc, IID_PPV_ARGS( &m_pID3D12CommandQueue ) );
	ThrowIfFailedMsg( hr, "Failed to create command queue!" );
}

void FCommandQueue::CreateSyncObjects()
{
	ID3D12Device* pD3D12Device = (ID3D12Device*)m_pRenderDeviceRef->GetNativeDevice();
	HRESULT hr = S_OK;

	hr = pD3D12Device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &m_pFence ) );
	ThrowIfFailedMsg( hr, "Failed to create fence object!" );
	m_pFence->SetName( L"CommandListManager::m_pFence" );

	D3D12_COMMAND_LIST_TYPE Type = (D3D12_COMMAND_LIST_TYPE)m_Type;
	hr = m_pFence->Signal( SCast<uint64>( Type ) << 56 );
	ThrowIfFailedMsg( hr, "Failed to signal fence object!" );

	m_FenceEventHandle = CreateEvent( nullptr, false, false, nullptr );
	HE_ASSERT( m_FenceEventHandle != NULL );
}

#endif // R_WITH_D3D12
