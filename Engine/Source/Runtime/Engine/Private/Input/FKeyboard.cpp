// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/FKeyboard.h"


FKeyboard::FKeyboard()
	: m_IsAcquired( false )
{

#if HE_INPUT_USE_XINPUT
	m_pDIDKeyboard = NULL;
#endif

	BuildKeyBuffer();
}

FKeyboard::~FKeyboard()
{

}

void FKeyboard::SetupPlatformRIDProvider( void* pProviderInterface, void* pNativeWindow )
{
#if HE_INPUT_USE_XINPUT
	IDirectInput8A* pDirectInput = RCast<IDirectInput8A*>( pProviderInterface );
	HE_ASSERT( pDirectInput != NULL );
	HWND phNativeWindow = RCast<HWND>( pNativeWindow );
	HE_ASSERT( phNativeWindow != NULL );

	HRESULT hr = pDirectInput->CreateDevice( GUID_SysKeyboard, &m_pDIDKeyboard, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard CreateDevice failed.

	hr = m_pDIDKeyboard->SetDataFormat( &c_dfDIKeyboard );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard SetDataFormat failed.

	hr = m_pDIDKeyboard->SetCooperativeLevel( phNativeWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard SetCooperativeLevel failed.

	DIPROPDWORD dipdw = {};
	dipdw.diph.dwSize		= sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= 10;
	hr = m_pDIDKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard set buffer size failed.
#endif
}
