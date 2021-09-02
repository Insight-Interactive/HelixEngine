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

}

void HWorld::Initialize()
{
	HLevel* Level = m_Levels.emplace_back( new HLevel(this) );
	Level->LoadFromFile("TODO: Content/Levels/Test.hlevel");
}

float HWorld::GetDeltaTime() const
{
	return GEngine->GetDeltaTime();
}

void HWorld::BeginPlay()
{
	for (uint64 i = 0; i < m_Levels.size(); ++i)
	{
		m_Levels[i]->BeginPlay();
	}
}

void HWorld::Tick(float DeltaTime)
{
	m_CameraManager.Tick( DeltaTime );

	for (uint64 i = 0; i < m_Levels.size(); ++i)
	{
		m_Levels[i]->Tick(DeltaTime);
	}
}

void HWorld::Flush()
{
	for (uint64 i = 0; i < m_Levels.size(); ++i)
	{
		m_Levels[i]->Flush();
	}
}

void HWorld::Render(ICommandContext& CmdContext)
{
	for (uint64 i = 0; i < m_Levels.size(); ++i)
	{
		m_Levels[i]->Render(CmdContext);
	}
}
