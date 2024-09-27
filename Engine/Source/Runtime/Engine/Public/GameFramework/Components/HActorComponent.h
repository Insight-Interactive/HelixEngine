// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

/*
	Autogenerate the required methods needed to derive from HActorComponent.
*/
#define HE_COMPONENT_GENERATED_BODY( Class )													\
	Class( FComponentInitArgs& InitArgs );														\
	virtual ~Class();																			\
	friend class FActorSerializer;																\
public:																							\
	virtual const char* GetComponenetStaticName() override { return HE_STRINGIFY( Class ); }	\

#define HCOMPONENT()


class AActor;
class HWorld;
class FCommandContext;

/*
	Common parameters used to initialize a new component onto an actor.
*/
struct FComponentInitArgs
{
	FComponentInitArgs( const char* Name, AActor* pOwner )
	{
		strcpy_s( this->Name, Name );
		this->pOwner = pOwner;
	}
	char Name[kMaxHObjectNameLength];
	const AActor*	pOwner;
};

/*
	Base class for all actor components. An Actor is defined by multiple 
	components which bestow behavior to it.
*/
HCOMPONENT()
class HActorComponent : public HObject
{
	friend class HLevel;
	friend class AActor;
	friend class FActorSerializer;
public:
	// Returns the Actor class this component belongs too.
	AActor* GetOwner();

	// Returns the class name of this componenent. Override for behavior.
	virtual const char* GetComponenetStaticName() = 0;

protected:
	HActorComponent( FComponentInitArgs& InitArgs );
	virtual ~HActorComponent();


	// Returns the world the component resides in.
	HWorld* GetWorld();

	// Called on the first frame of the game.
	virtual void BeginPlay() {}
	// Called once every frame.
	virtual void Tick( float DeltaTime ) {}

	// Called when the component is initialy created.
	virtual void OnCreate() {}
	// Called when the component is attached to its owning actor.
	virtual void OnAttach() {}
	// Called when the component is detached from its owning actor.
	virtual void OnDetach() {}
	// Called when the component is destroyed and released from memory.
	virtual void OnDestroy() {}
	virtual void Render( FCommandContext& GfxContext ) {}
	// Called when the owning actor has completly finished deserializing all other components it owns.
	virtual void OnOwnerDeserializeComplete();

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;


protected:
	AActor* m_Owner;

};


//
// Inline function implmentations
//


FORCEINLINE AActor* HActorComponent::GetOwner()
{
	return m_Owner;
}
