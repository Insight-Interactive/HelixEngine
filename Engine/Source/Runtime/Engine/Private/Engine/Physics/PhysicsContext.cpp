#include "EnginePCH.h"

#include "Engine/Physics/PhysicsContext.h"


static physx::PxDefaultErrorCallback SDefaultErrorCallback;
static physx::PxDefaultAllocator SDefaultAllocatorCallback;

// HPhysicsContext
//

HPhysicsContext::HPhysicsContext()
	: m_pFoundation( nullptr )
	, m_pPhysics( nullptr )
	, m_pDispatcher( nullptr )
	, m_pCooker( nullptr )
	, m_pVisualDebugger( nullptr )
{

}

HPhysicsContext::~HPhysicsContext()
{

}

void HPhysicsContext::Initialize()
{
	HE_ASSERT( !IsReady() ); // Physics context has already been initialized!

	CreateLogger( GPhysicsLogger, "Physics Engine" );

	m_pFoundation = PxCreateFoundation( PX_PHYSICS_VERSION, SDefaultAllocatorCallback, SDefaultErrorCallback );
	HE_ASSERT( m_pFoundation != nullptr );

#if HE_WIN64
#	if HE_DEBUG_PHYSICS
		m_pVisualDebugger = PxCreatePvd( *m_pFoundation );
		HE_ASSERT( m_pVisualDebugger != nullptr );
		const char* PVD_HOST = "127.0.0.1";
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate( PVD_HOST, 5425, 10 );
		m_pVisualDebugger->connect( *transport, physx::PxPvdInstrumentationFlag::eALL );
#	endif // HE_DEBUG_PHYSICS
#else
//#	pragma message ("Compiling in non-64-bit configuration. PhysX Visual Debugger will not be available.") 
#endif // HE_WIN64
	
	physx::PxTolerancesScale Tolerance = {};
	m_pPhysics = PxCreatePhysics( PX_PHYSICS_VERSION, *m_pFoundation, Tolerance, bool(P_TRACK_MEMORY_ALLOCATIONS), m_pVisualDebugger );
	HE_ASSERT( m_pPhysics != nullptr );

	m_pCooker = PxCreateCooking( PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams( Tolerance ) );
	HE_ASSERT( m_pCooker != nullptr );
	
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate( 1 );
	HE_ASSERT( m_pDispatcher != nullptr );

#if HE_WIN64
	bool ExtensionsLoadSuccess = PxInitExtensions( *m_pPhysics, m_pVisualDebugger );
	HE_ASSERT( ExtensionsLoadSuccess == true );
#else
#	pragma message ("Compileing in non-64-bit configuration. PhysX Extentions will not be available.") 
#endif

}

void HPhysicsContext::UnInitialize()
{
	PX_SAFE_RELEASE( m_pCooker );
	PX_SAFE_RELEASE( m_pPhysics );
#if HE_WIN64
#	if HE_DEBUG_PHYSICS
	if (m_pVisualDebugger)
	{
		physx::PxPvdTransport* pTransport = m_pVisualDebugger->getTransport();
		//PX_SAFE_RELEASE( pTransport ); // TODO: Causing error, why?
		m_pVisualDebugger->release();
	}
#	endif // HE_DEBUG_PHYSICS
#endif // HE_WIN64

	PX_SAFE_RELEASE( m_pFoundation );
}

// physx::PxDefaultErrorCallback Implementation
//

physx::PxDefaultErrorCallback::PxDefaultErrorCallback()
{

}

physx::PxDefaultErrorCallback::~PxDefaultErrorCallback()
{

}

void physx::PxDefaultErrorCallback::reportError( physx::PxErrorCode::Enum code, const char* message, const char* file, int line )
{
	ELogSeverity Severity = ELogSeverity::Log;
	switch (code)
	{
	case physx::PxErrorCode::eNO_ERROR:
	case physx::PxErrorCode::eDEBUG_INFO:
		Severity = ELogSeverity::Log;
		break;
	case physx::PxErrorCode::ePERF_WARNING:
	case physx::PxErrorCode::eINVALID_PARAMETER:
	case physx::PxErrorCode::eDEBUG_WARNING:
		Severity = ELogSeverity::Warning;
		break;
	case physx::PxErrorCode::eINVALID_OPERATION:
	case physx::PxErrorCode::eOUT_OF_MEMORY:
	case physx::PxErrorCode::eINTERNAL_ERROR:
	case physx::PxErrorCode::eABORT:
		Severity = ELogSeverity::Error;
		break;
	case physx::PxErrorCode::eMASK_ALL:
		break;
	default:
		break;
	}
	P_LOG_EXPLICIT_SEVERITY( Severity, "[PhysX-%s-%i] %s", file, line, message );
}