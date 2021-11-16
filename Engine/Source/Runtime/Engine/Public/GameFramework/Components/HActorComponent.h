// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"
#include "AssetRegistry/SerializeableInterface.h"


#define HE_COMPONENT_GENERATED_BODY( Class )	\
			Class( const HName& Name );			\
			virtual ~Class();

#define HCOMPONENT()


class AActor;
class HWorld;
class FCommandContext;

HCOMPONENT()
class HActorComponent : public HObject, public FSerializeableInterface
{
	friend class AActor;
public:
	inline AActor* GetOwner() const;

protected:
	HE_COMPONENT_GENERATED_BODY( HActorComponent )
	

	void SetOwner( AActor* pNewOwner );
	AActor* GetOwner();
	HWorld* GetWorld();

	virtual void BeginPlay() {}
	virtual void Tick( float DeltaTime ) {}

	virtual void OnCreate() {}
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnDestroy() {}
	virtual void Render( FCommandContext& GfxContext ) {}

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	AActor* m_pOwner;

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
