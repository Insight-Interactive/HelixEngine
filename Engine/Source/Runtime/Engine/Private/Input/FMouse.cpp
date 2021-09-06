// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/FMouse.h"


FMouse::FMouse()
	: m_IsAcquired( false )
{
#if HE_INPUT_USE_XINPUT
	m_pDIDMouse = NULL;
#endif

	ZeroMemory( m_MouseMoveDelta[0], sizeof( m_MouseMoveDelta[0] ) );
	ZeroMemory( m_MouseMoveDelta[1], sizeof( m_MouseMoveDelta[1] ) );
}

FMouse::~FMouse()
{
	Destroy();
}

void FMouse::SetupPlatformRIDProvider(void* pProviderInterface, void* pNativeWindow)
{
#if HE_INPUT_USE_XINPUT
	IDirectInput8A* pDirectInput = RCast<IDirectInput8A*>( pProviderInterface );
	HE_ASSERT( pDirectInput != NULL );
	HWND phNativeWindow = RCast<HWND>( pNativeWindow );
	HE_ASSERT( phNativeWindow != NULL );

	HRESULT hr = pDirectInput->CreateDevice( GUID_SysMouse, &m_pDIDMouse, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse CreateDevice failed.

	hr = m_pDIDMouse->SetDataFormat( &c_dfDIMouse2 );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse SetDataFormat failed.

	hr = m_pDIDMouse->SetCooperativeLevel( phNativeWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse SetCooperativeLevel failed.
#endif
}