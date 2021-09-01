// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


class AActor;
class HWorld;
class ICommandContext;

class HActorComponent
{
	friend class AActor;
public:
	inline AActor* GetOwner() const;

protected:
	HActorComponent( const HName& Name )
		: m_pOwner( NULL )
		, m_Name( Name )

	{
	}
	virtual ~HActorComponent()
	{
	}

	void SetOwner( AActor* pNewOwner );
	AActor* GetOwner();
	HWorld* GetWorld();

	virtual void BeginPlay() {}
	virtual void Tick( float DeltaTime ) {}

	virtual void OnCreate() {}
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnDestroy() {}
	virtual void Render( ICommandContext& GfxContext ) = 0;

protected:
	AActor* m_pOwner;
	HName m_Name;
};


//
// Inline function implmentations
//

inline AActor* HActorComponent::GetOwner() const
{
	return m_pOwner;
}

inline void HActorComponent::SetOwner( AActor* pNewOwner )
{
	m_pOwner = pNewOwner;
}

inline AActor* HActorComponent::GetOwner()
{
	return m_pOwner;
}
