// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


#define HE_GENERATED_BODY()


class HWorld;
class HActorComponent;
class ICommandContext;

class AActor
{
	friend class HLevel;
	friend class HActorComponent;
public:
	AActor( HWorld* pWorld );
	virtual ~AActor();

	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	template <typename ComponentType, typename ... InitArgs>
	inline ComponentType* AddComponent( InitArgs ... args );

	void RemoveAllComponents();

protected:
	void Render( ICommandContext& GfxContext );
	inline HWorld* GetWorld();
	inline void SetOwningWorld( HWorld* pWorld );

protected:
	TDynamicArray<HActorComponent*> m_Components;
	HWorld* m_pOwningWorld;
};


//
// Inline function implementations
//

template <typename ComponentType, typename ... InitArgs>
inline ComponentType* AActor::AddComponent( InitArgs ... args )
{
	HActorComponent* pNewComponent = new ComponentType( args... );
	pNewComponent->SetOwner( this );
	pNewComponent->OnCreate();
	pNewComponent->OnAttach();

	m_Components.PushBack( pNewComponent );

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
