// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/AActor.h"



AActor::AActor( FActorInitArgs& InitArgs )
	: HObject( InitArgs.Name )
	, m_pRootComponent( nullptr )
	, m_pOwningWorld( CCast<HWorld*>( InitArgs.pWorld ) )
	, m_IsDynamicInstance( InitArgs.bIsDynamicInstance )
{
}

AActor::~AActor()
{

}

void AActor::OnDeserializeComplete()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->OnOwnerDeserializeComplete();
	}
}

void AActor::RemoveAllComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->OnDetach();
		m_Components[i]->OnDestroy();
		delete m_Components[i];
	}
	m_Components.clear();
}

void AActor::BeginPlay()
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->BeginPlay();
	}
}

void AActor::Tick( float DeltaTime )
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Tick( DeltaTime );
	}
}

void AActor::Render( FCommandContext& GfxContext )
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Render( GfxContext );
	}
}


#if HE_WITH_EDITOR

void AActor::OnEditorSelected()
{
	HE_LOG( Log, TEXT( "Selected Actor: %s" ), GetObjectName() );
}

#endif