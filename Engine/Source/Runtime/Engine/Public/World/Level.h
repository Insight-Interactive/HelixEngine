// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Flag.h"
#include "CriticalSection.h"
#include "GameFramework/HObject.h"
#include "GameFramework/Actor/AActor.h"

class AActor;
class HWorld;
class FCommandContext;

class HLevel : public HObject
{
	friend class HWorld;
	friend class WorldOutlinePanel;
	using Super = HObject;
public:
	HLevel(HWorld* pOwner);
	~HLevel();

	void Flush();
	bool IsValid() const;
	bool IsLoading() const;

	template <typename ActorType>
	ActorType* CreateActor(const char* Name = "<Unnamed Actor>");

	AActor* GetActor( const char* Name ) const
	{
		StringHashValue NameHash = StringHash( Name );
		for (uint32 i = 0; i < m_Actors.size(); i++)
		{
			AActor& Actor = *m_Actors[i];
			if ( StringHash( Actor.GetObjectName() ) == NameHash )
				return &Actor;
		}

		return nullptr;
	}

	std::vector<AActor*>& GetAllActors() { return m_Actors; }

	HWorld* GetWorld();

protected:
	void GuardedAddActor(AActor* pActor);

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(FCommandContext& CmdContext);

	virtual void Serialize( JsonUtility::WriteContext& Value ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

protected:
	std::vector<AActor*> m_Actors;
	CriticalSection m_ActorListGuard;
	HWorld* m_pOwningWorld;

	FFlag m_IsLoading;

};


//
// Inline function implementations
//

template <typename ActorType>
FORCEINLINE ActorType* HLevel::CreateActor(const char* Name)
{
	HE_ASSERT( m_pOwningWorld != NULL ); // Cannot add an actor to a level with a null world!

	FActorInitArgs InitArgs{ m_pOwningWorld, Name, false };
	AActor* pNewActor = new ActorType( InitArgs);
	HE_ASSERT(pNewActor != NULL);

	GuardedAddActor(pNewActor);

	return SCast<ActorType*>(pNewActor);
}

FORCEINLINE void HLevel::GuardedAddActor(AActor* pActor)
{
	ScopedCriticalSection Guard( m_ActorListGuard );
	m_Actors.push_back(pActor);
}

FORCEINLINE HWorld* HLevel::GetWorld()
{
	return m_pOwningWorld;
}

FORCEINLINE bool HLevel::IsValid() const
{
	return m_pOwningWorld && m_Actors.size() > 0;
}

FORCEINLINE bool HLevel::IsLoading() const
{
	return m_IsLoading.IsSet();
}
