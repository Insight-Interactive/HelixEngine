// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/Asset.h"
#include "GameFramework/HObject.h"
#include "GameFramework/Components/HSceneComponent.h"
#include "AssetRegistry/SerializeableInterface.h"
#include "LuaScript.h"

#define HE_GENERATED_BODY( Class )				\
			Class( FActorInitArgs& InitArgs );	\
			virtual ~Class();							

#define HCLASS()

struct FActorInitArgs
{
	const HWorld*	pWorld;
	const HName&	Name;
	const bool		bIsDynamicInstance;
	const bool		bDisableCollision = false;
};

class HWorld;
class HActorComponent;
class FCommandContext;

HCLASS()
class AActor : public HObject, public FSerializeableInterface
{
	friend class HLevel;
	friend class HWorld;
	friend class HActorComponent;
	friend class DetailsPanel;
	using Super = HObject;
public:
	HE_GENERATED_BODY( AActor )

	// Called once when the game has started playing.
	virtual void BeginPlay();
	// Called once per frame.
	virtual void Tick( float DeltaTime );

	// Add a component to the actor.
	template <typename ComponentType>
	ComponentType* AddComponent(const HName& Name);
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

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;
	virtual void OnDeserializeComplete();

#if HE_WITH_EDITOR
	virtual void OnEditorSelected();
#endif

protected:
	std::vector<HActorComponent*> m_Components;
	HSceneComponent* m_pRootComponent;

	HWorld* m_pOwningWorld;

	LuaScript m_Script;
	bool m_IsDynamicInstance;
};


//
// Inline function implementations
//

template <typename ComponentType>
FORCEINLINE ComponentType* AActor::AddComponent( const HName& Name )
{
	// Create, initialize and attach the component to the actor.
	FComponentInitArgs InitArgs{ Name, this };
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
	return m_pOwningWorld;
}

FORCEINLINE void AActor::SetOwningWorld( HWorld* pWorld )
{
	m_pOwningWorld = pWorld;
}

FORCEINLINE void AActor::SetRootComponent(HSceneComponent* pRoot)
{
	m_pRootComponent = pRoot;
}

FORCEINLINE HSceneComponent* AActor::GetRootComponent()
{
	return m_pRootComponent;
}

FORCEINLINE LuaScript& AActor::GetScript()
{
	return m_Script;
}
