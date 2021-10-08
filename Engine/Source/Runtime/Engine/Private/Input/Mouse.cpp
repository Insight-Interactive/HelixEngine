// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/Mouse.h"


FMouse::FMouse()
	: m_IsAcquired( false )
{
	ZeroMemory( m_MouseState.Buttons, sizeof( m_MouseState.Buttons ) );
	ZeroMemory( m_MouseState.MoveDelta[kState_Current],  sizeof( m_MouseState.MoveDelta[kState_Current] ) );
	ZeroMemory( m_MouseState.MoveDelta[kState_Previous], sizeof( m_MouseState.MoveDelta[kState_Previous] ) );
}

FMouse::~FMouse()
{
	Destroy();
}

void FMouse::SetupPlatformHIDProvider(void* pProviderInterface, void* pNativeWindow)
{
#if HE_WINDOWS_DESKTOP
	// Do Nothing ...
#endif
}