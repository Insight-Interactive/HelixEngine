#include "RendererPCH.h"

#include "CommandManagerD3D12.h"
#include "IDevice.h"
#include "ICommandContext.h"
#include "RendererCore.h"


// ---------------------
// D3D12 Command Manager
// ---------------------

void CommandManagerD3D12::Initialize( IDevice* pDevice )
{
	m_pDeviceRef = pDevice;
	HE_ASSERT( m_pDeviceRef != NULL );

	m_pID3D12DeviceRef = RCast<ID3D12Device*>( m_pDeviceRef->GetNativeDevice() );

	//
	// Create the Command Queues
	//
	m_pGraphicsQueue = &m_D3D12GraphicsQueue;
	m_D3D12GraphicsQueue.Initialize( m_pID3D12DeviceRef );
	// TODO: Create command queue
}

void CommandManagerD3D12::UnInitialize()
{
	m_pD3D12DeviceRef = NULL;
	m_pID3D12DeviceRef = NULL;
}

void CommandManagerD3D12::CreateNewCommandContext( const ECommandListType& Type, ICommandContext** pContext, void** pCommandAllocator )
{
	switch (Type)
	{
	case ECommandListType::CLT_Direct:
		(*pCommandAllocator) = m_D3D12GraphicsQueue.RequestAllocator();
		break;
	case ECommandListType::CLT_Compute:
		break;
	default:
		break;
	}

	ID3D12GraphicsCommandList** pD3D12CmdList = RCast<ID3D12GraphicsCommandList**>( (*pContext)->GetNativeContextAddress() );

	HRESULT hr = m_pID3D12DeviceRef->CreateCommandList( 0, (D3D12_COMMAND_LIST_TYPE)Type, RCast<ID3D12CommandAllocator*>( *pCommandAllocator ), NULL, IID_PPV_ARGS( pD3D12CmdList ) );
	ThrowIfFailedMsg( hr, TEXT( "Failed to create command list!" ) );
}

void CommandManagerD3D12::WaitForFence( uint64 FenceValue )
{
	ICommandQueue* Queue = GCommandManager->GetQueue( (ECommandListType)(FenceValue >> 56) );
	Queue->WaitForFence( FenceValue );
}



// -----------------------
//  D3D12 Command Queue
// -----------------------

CommandQueueD3D12::CommandQueueD3D12( const ECommandListType Type )
	: ICommandQueue( Type )
	, m_pID3D12CommandQueue( NULL )
	, m_pID3DDeviceRef( NULL )
	, m_D3D12AllocatorPool( (D3D12_COMMAND_LIST_TYPE)Type )
	, m_pFence( NULL )
	, m_NextFenceValue( 0u )
	, m_LastCompletedFenceValue( 0u )
	, m_FenceEventHandle( NULL )
{
}

uint64 CommandQueueD3D12::ExecuteCommandList( ID3D12CommandList* pCommandList )
{
	m_FenceMutex.Enter();

	HRESULT hr = ((ID3D12GraphicsCommandList*)pCommandList)->Close();
	ThrowIfFailedMsg( hr, TEXT( "Failed to close command list!" ) );

	m_pID3D12CommandQueue->ExecuteCommandLists( 1, &pCommandList );

	m_pID3D12CommandQueue->Signal( m_pFence, m_NextFenceValue );

	m_FenceMutex.Exit();
	return m_NextFenceValue++;
}

void CommandQueueD3D12::WaitForFence( uint64 FenceValue )
{
	if (IsFenceCompleted( FenceValue ))
		return;

	{
		m_EventMutex.Enter();

		m_pFence->SetEventOnCompletion( FenceValue, m_FenceEventHandle );
		WaitForSingleObject( m_FenceEventHandle, INFINITE );
		m_LastCompletedFenceValue = FenceValue;

		m_EventMutex.Exit();
	}
}

uint64 CommandQueueD3D12::IncrementFence()
{
	m_FenceMutex.Enter();
	m_pID3D12CommandQueue->Signal( m_pFence, m_NextFenceValue );
	m_FenceMutex.Exit();
	return m_NextFenceValue++;
}

bool CommandQueueD3D12::IsFenceCompleted( uint64 FenceValue )
{
	if (FenceValue > m_LastCompletedFenceValue)
		m_LastCompletedFenceValue = HE_MAX( m_LastCompletedFenceValue, m_pFence->GetCompletedValue() );

	return FenceValue <= m_LastCompletedFenceValue;
}

CommandQueueD3D12::~CommandQueueD3D12()
{
	CloseHandle( m_FenceEventHandle );

	HE_COM_SAFE_RELEASE( m_pFence );
	HE_COM_SAFE_RELEASE( m_pID3D12CommandQueue );
}

void CommandQueueD3D12::Initialize( ID3D12Device* pID3D12Device )
{
	HE_ASSERT( pID3D12Device != NULL );
	m_pID3DDeviceRef = pID3D12Device;

	m_D3D12AllocatorPool.Initialize( pID3D12Device );
	CreateD3D12Queue();
	CreateSyncObjects();
}

void CommandQueueD3D12::CreateD3D12Queue()
{
	D3D12_COMMAND_QUEUE_DESC Desc;
	ZeroMemory( &Desc, sizeof( D3D12_COMMAND_QUEUE_DESC ) );
	Desc.Type = (D3D12_COMMAND_LIST_TYPE)m_Type;
	Desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	HRESULT hr = m_pID3DDeviceRef->CreateCommandQueue( &Desc, IID_PPV_ARGS( &m_pID3D12CommandQueue ) );
	ThrowIfFailedMsg( hr, TEXT( "Failed to create command queue!" ) );
}

void CommandQueueD3D12::CreateSyncObjects()
{
	HRESULT hr = S_OK;

	hr = m_pID3DDeviceRef->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &m_pFence ) );
	ThrowIfFailedMsg( hr, TEXT( "Failed to create fence object!" ) );
	m_pFence->SetName( L"CommandListManager::m_pFence" );

	D3D12_COMMAND_LIST_TYPE Type = (D3D12_COMMAND_LIST_TYPE)m_Type;
	hr = m_pFence->Signal( SCast<uint64>( Type ) << 56 );
	ThrowIfFailedMsg( hr, TEXT( "Failed to signal fence object!" ) );

	m_FenceEventHandle = CreateEvent( nullptr, false, false, nullptr );
	HE_ASSERT( m_FenceEventHandle != NULL );
}
