// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Subsystem/PhysicsSubsystem.h"

#include "App/App.h"


FPhysicsSubsystem::FPhysicsSubsystem()
	: FSubsystemInterface("Physics Subsystem")
{
}

FPhysicsSubsystem::~FPhysicsSubsystem()
{
}

void FPhysicsSubsystem::Initialize()
{

}

void FPhysicsSubsystem::UnInitialize()
{
	TerminateAsyncProcess();
}

void FPhysicsSubsystem::RunAsync_Implementation()
{
	while (m_IsRunning)
	{
	}

	HE_LOG( Log, TEXT( "Exiting physics subsystem async run loop." ) );
}
