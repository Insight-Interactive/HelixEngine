// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/Keyboard.h"


FKeyboard::FKeyboard()
	: m_IsAcquired( false )
{
	ZeroMemory( m_KeyBuffer, sizeof( m_KeyBuffer ) );

	BuildKeyBuffer();
}

FKeyboard::~FKeyboard()
{

}

void FKeyboard::SetupPlatformHIDProvider( void* pProviderInterface, void* pNativeWindow )
{
}
