// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "App/FApp.h"


FApp::FApp()
	: m_IsRunning( true )
{
	ZeroMemory( m_Name, sizeof( m_Name ) );
}

FApp::~FApp()
{

}

void FApp::Startup()
{
	const TChar* TargetName =
#if HE_WITH_EDITOR
		TEXT( "Helix Ed" ) " [" HE_PLATFORM_STRING " - " HE_CONFIG_STRING "]";
#else
		TEXT( "My Application" );
#endif
	TCharStrCpy( m_Name, TargetName );
}

void FApp::Shutdown()
{

}

