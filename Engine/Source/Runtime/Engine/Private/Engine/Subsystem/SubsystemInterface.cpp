// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Subsystem/SubsystemInterface.h"

static void SubsystemRunAsyncHelper( void* pClass );

FSubsystemInterface::FSubsystemInterface( Char* DebugName )
	: m_IsRunning( false )
{
#if HE_DEBUG
	strcpy_s( m_DebugName, DebugName );
#endif // HE_DEBUG
}

FSubsystemInterface::~FSubsystemInterface()
{

}

void FSubsystemInterface::RunAsync()
{
	Char ThreadName[256];
	ZeroMemory( ThreadName, sizeof( ThreadName ) );
#if HE_DEBUG
	sprintf_s( ThreadName, "Helix Engine Subsystem Thread: %s", m_DebugName );
#endif
	System::CreateAndRunThread( ThreadName, 0, SubsystemRunAsyncHelper, this );

	m_IsRunning = true;
}


// Static methods

class FSubsystemAsyncRunHelper
{
public:
	FORCEINLINE FSubsystemAsyncRunHelper( FSubsystemInterface* pSubsystem )
	{
		HE_ASSERT( pSubsystem != nullptr );
		pSubsystem->RunAsync_Implementation();
	}
};

/*static*/ void SubsystemRunAsyncHelper( void* pClass )
{
	FSubsystemInterface* pSubsystem = SCast< FSubsystemInterface*>( pClass );

	FSubsystemAsyncRunHelper Runner( pSubsystem );
}
