// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "App/FApp.h"
#include "Engine/GameProject.h"


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
}

void FApp::Shutdown()
{

}

