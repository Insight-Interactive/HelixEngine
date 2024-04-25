// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Subsystem/PhysicsSubsystem.h"

#include "PhysicsScene.h"
#include "Engine/FrameTimeManager.h"


FPhysicsSubsystem::FPhysicsSubsystem()
	: FSubsystemInterface( "Physics Subsystem" )
{
}

FPhysicsSubsystem::~FPhysicsSubsystem()
{
}

void FPhysicsSubsystem::Initialize()
{
	m_Scenes.reserve( 1 );
	m_PhysicsContext.Initialize();
}

void FPhysicsSubsystem::UnInitialize()
{
	TerminateAsyncProcess();
	m_PhysicsContext.UnInitialize();
}

void FPhysicsSubsystem::RunAsync_Implementation()
{
	return;
	while (m_IsRunning)
	{
		for (HPhysicsScene* pScene : m_Scenes)
		{
			pScene->ProcessEventQueue();
		}
	}

	HE_LOG( Log, TEXT( "Exiting physics subsystem async run loop." ) );
}
