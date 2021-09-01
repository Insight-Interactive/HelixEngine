// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/AActor.h"

#include "ICommandContext.h"
#include "GameFramework/Actor/Components/HActorComponent.h"


AActor::AActor( HWorld* pWorld )
	: m_pOwningWorld( pWorld )
{

}

AActor::~AActor()
{

}

void AActor::RemoveAllComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->OnDetach();
		m_Components[i]->OnDestroy();
		delete m_Components[i];
	}
}

void AActor::BeginPlay()
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->BeginPlay();
	}
}

void AActor::Tick(float DeltaTime)
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Tick(DeltaTime);
	}
}

void AActor::Render(ICommandContext& GfxContext)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Render(GfxContext);
	}
}
