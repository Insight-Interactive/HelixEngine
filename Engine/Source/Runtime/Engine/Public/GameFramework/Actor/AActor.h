// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "LuaScript.h"
#include "GameFramework/Components/HActorComponent.h"


#define HE_GENERATED_BODY( Class )															\
	Class( FActorInitArgs& InitArgs );														\
	virtual ~Class();																		\
	virtual const char* GetStaticClassName() override { return HE_STRINGIFY( Class ); }		\

#define HCLASS()

class HWorld;
class HSceneComponent;
class FCommandContext;

struct FActorInitArgs
{
	const HWorld*	pWorld;
	const char*		Name;
	const bool		bIsDynamicInstance;
	const bool		bDisableCollision = false;
};

HCLASS()
class AActor : public HObject
{
	friend class HLevel;
	friend class HWorld;
	friend class HActorComponent;
	friend class DetailsPanel;
	friend class FActorSerializer;
	using Super = HObject;
public:
	AActor( FActorInitArgs& InitArgs );
	virtual ~AActor();
	virtual const char* GetStaticClassName() { return HE_STRINGIFY( AActor ); }


	// Called once when the game has started playing.
	virtual void BeginPlay();
	// Called once per frame.
	virtual void Tick( float DeltaTime );
	// Garenteed to be called 60 times in one second.
	virtual void FixedUpdate( float Time );

	// Add a component to the actor.
	template <typename ComponentType>
	ComponentType* AddComponent(const char* Name);
	template <typename ComponentType>
	ComponentType* GetComponent();
	// Remove all components from the actor.
	void RemoveAllComponents();
	// Finds and returns a component by its unique id. nullptr if the component is not found.
	HActorComponent* GetComponentByGuid(const FGUID& Guid);

	void SetRootComponent(HSceneComponent* pRoot);
	HSceneComponent* GetRootComponent();

	LuaScript& GetScript();


protected:
	void Render( FCommandContext& GfxContext );
	HWorld* GetWorld();
	void SetOwningWorld( HWorld* pWorld );

	virtual void OnDeserializeComplete();

#if HE_WITH_EDITOR
	virtual void OnEditorSelected();
#endif

protected:
	std::vector<HActorComponent*> m_Components;
	HSceneComponent* m_RootComponent;

	HWorld* m_OwningWorld;

	LuaScript m_Script;
	bool m_IsDynamicInstance;
};


//
// Inline function implementations
//

template <typename ComponentType>
FORCEINLINE ComponentType* AActor::AddComponent( const char* Name )
{
	// Create, initialize and attach the component to the actor.
	FComponentInitArgs InitArgs( Name, this );
	HActorComponent* pNewComponent = new ComponentType(InitArgs);
	pNewComponent->OnCreate();
	pNewComponent->OnAttach();

	m_Components.push_back( pNewComponent );

	return SCast<ComponentType*>( pNewComponent );
}

template <typename ComponentType>
ComponentType* AActor::GetComponent()
{
	for (auto* Iter : m_Components)
	{
		if (DCast<ComponentType*>( Iter ) != nullptr)
			return (ComponentType*)Iter;
	}
	return nullptr;
}

FORCEINLINE HActorComponent* AActor::GetComponentByGuid(const FGUID& Guid)
{
	for (auto* Iter : m_Components)
	{
		if (Iter->GetGuid() == Guid)
			return Iter;
	}
	return nullptr;
}

FORCEINLINE HWorld* AActor::GetWorld()
{
	return m_OwningWorld;
}

FORCEINLINE void AActor::SetOwningWorld( HWorld* pWorld )
{
	m_OwningWorld = pWorld;
}

FORCEINLINE void AActor::SetRootComponent(HSceneComponent* pRoot)
{
	m_RootComponent = pRoot;
}

FORCEINLINE HSceneComponent* AActor::GetRootComponent()
{
	return m_RootComponent;
}

FORCEINLINE LuaScript& AActor::GetScript()
{
	return m_Script;
}
