// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


#define HE_GENERATED_BODY( Class )		\
			Class( HWorld* pWorld );	\
			virtual ~Class();

class HWorld;
class HActorComponent;
class ICommandContext;

class AActor
{
	friend class HLevel;
	friend class HActorComponent;
public:
	HE_GENERATED_BODY( AActor )


	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	template <typename ComponentType, typename ... InitArgs>
	inline ComponentType* AddComponent( InitArgs ... args );

	void RemoveAllComponents();

	void SetDisplayName( const HName& Name );
	const HName& GetDisplayName() const;

protected:
	void Render( ICommandContext& GfxContext );
	inline HWorld* GetWorld();
	inline void SetOwningWorld( HWorld* pWorld );

protected:
	std::vector<HActorComponent*> m_Components;
	HWorld* m_pOwningWorld;
	HName m_DisplayName;
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

inline void AActor::SetDisplayName( const HName& Name )
{
	m_DisplayName = Name;
}

inline const HName& AActor::GetDisplayName() const
{
	return m_DisplayName;
}