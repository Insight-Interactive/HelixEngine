// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/HWorld.h"

#include "World/HLevel.h"
#include "Engine/HEngine.h"


HWorld::HWorld()
	: m_CameraManager(this)
{
}

HWorld::~HWorld()
{
	Flush();
}

void HWorld::Initialize()
{
	m_pLevel = new HLevel( this );
	m_pLevel->LoadFromFile("Content/Levels/TestLevel.hlevel");
}

float HWorld::GetDeltaTime() const
{
	return GEngine->GetDeltaTime();
}

void HWorld::BeginPlay()
{
	m_pLevel->BeginPlay();
}

void HWorld::Tick(float DeltaTime)
{
	m_CameraManager.Tick( DeltaTime );

	m_pLevel->Tick(DeltaTime);
}

void HWorld::Flush()
{
	if (m_pLevel != NULL)
	{
		m_pLevel->Flush();
		HE_SAFE_DELETE_PTR( m_pLevel );
	}
}

void HWorld::Render(ICommandContext& CmdContext)
{
	m_pLevel->Render(CmdContext);
}
