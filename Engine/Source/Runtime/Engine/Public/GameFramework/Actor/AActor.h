// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/Asset.h"
#include "GameFramework/HObject.h"
#include "AssetRegistry/SerializeableInterface.h"

#include "Transform.h"

#define HE_GENERATED_BODY( Class )																\
			Class( HWorld* pWorld, const HName& Name );	\
			virtual ~Class();							

#define HCLASS()


class HWorld;
class HActorComponent;
class FCommandContext;

HCLASS()
class AActor : public HObject, public FSerializeableInterface
{
	friend class HLevel;
	friend class HWorld;
	friend class HActorComponent;
public:
	HE_GENERATED_BODY( AActor )

	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	template <typename ComponentType, typename ... InitArgs>
	inline ComponentType* AddComponent( InitArgs ... args );

	void RemoveAllComponents();

	FTransform& GetTransform();

protected:
	void Render( FCommandContext& GfxContext );
	inline HWorld* GetWorld();
	inline void SetOwningWorld( HWorld* pWorld );

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

#if HE_WITH_EDITOR
	virtual void OnEditorSelected();
#endif

protected:
	std::vector<HActorComponent*> m_Components;
	HWorld* m_pOwningWorld;
	HName m_DisplayName;
	FTransform m_Transform;

};


//
// Inline function implementations
//

template <typename ComponentType, typename ... InitArgs>
inline ComponentType* AActor::AddComponent( InitArgs ... args )
{
	// Create, initialize and attach the component to the actor.
	HActorComponent* pNewComponent = new ComponentType( args... );
	pNewComponent->SetOwner( this );
	pNewComponent->OnCreate();
	pNewComponent->OnAttach();

	m_Components.push_back( pNewComponent );

	return SCast<ComponentType*>( pNewComponent );
}

inline HWorld* AActor::GetWorld()
{
	return m_pOwningWorld;
}

inline void AActor::SetOwningWorld( HWorld* pWorld )
{
	m_pOwningWorld = pWorld;
}

inline FTransform& AActor::GetTransform()
{
	return m_Transform;
}
