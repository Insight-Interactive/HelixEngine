// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/Asset.h"
#include "GameFramework/HObject.h"
#include "GameFramework/Components/HSceneComponent.h"
#include "AssetRegistry/SerializeableInterface.h"

#define HE_GENERATED_BODY( Class )																\
			Class( FActorInitArgs& InitArgs );	\
			virtual ~Class();							

#define HCLASS()

struct FActorInitArgs
{
	const HWorld*	pWorld;
	const HName&	Name;
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

	// Remove all components from the actor.
	void RemoveAllComponents();
	// Finds and returns a component by its unique id. nullptr if the component is not found.
	HActorComponent* GetComponentByGuid(const FGUID& Guid);


	void SetRootComponent(HSceneComponent* pRoot);
	HSceneComponent* GetRootComponent();

protected:
	void Render( FCommandContext& GfxContext );
	HWorld* GetWorld();
	void SetOwningWorld( HWorld* pWorld );

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

#if HE_WITH_EDITOR
	virtual void OnEditorSelected();
#endif

protected:
	std::vector<HActorComponent*> m_Components;
	HWorld* m_pOwningWorld;

	HSceneComponent* m_pRoot;

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
	m_pRoot = pRoot;
}

FORCEINLINE HSceneComponent* AActor::GetRootComponent()
{
	return m_pRoot;
}
