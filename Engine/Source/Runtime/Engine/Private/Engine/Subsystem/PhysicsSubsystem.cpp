// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Subsystem/PhysicsSubsystem.h"



FPhysicsSubsystem::FPhysicsSubsystem()
	: FSubsystemInterface( "Physics Subsystem" )
{
}

FPhysicsSubsystem::~FPhysicsSubsystem()
{
}

void FPhysicsSubsystem::Initialize()
{
	m_PhysicsContext.Initialize();
}

void FPhysicsSubsystem::UnInitialize()
{
	TerminateAsyncProcess();
	m_PhysicsContext.UnInitialize();
}

void FPhysicsSubsystem::RunAsync_Implementation()
{
	while (m_IsRunning)
	{
		const float StepRate = 1.f / 60.f;
		m_PhysicsContext.Tick( StepRate );
		m_PhysicsContext.QueryResults();
	}

	HE_LOG( Log, TEXT( "Exiting physics subsystem async run loop." ) );
}
