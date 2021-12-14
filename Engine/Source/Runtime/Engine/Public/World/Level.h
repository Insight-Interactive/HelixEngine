#pragma once

#include "CriticalSection.h"
#include "GameFramework/HObject.h"
#include "AssetRegistry/SerializeableInterface.h"


class AActor;
class HWorld;
class FCommandContext;

class HLevel : public HObject, public FSerializeableInterface
{
	friend class HWorld;
	friend class WorldOutlinePanel;
	using Super = HObject;
public:
	HLevel(HWorld* pOwner);
	~HLevel();

	void Flush();

	template <typename ActorType>
	ActorType* CreateActor(const HName& Name = TEXT("<Unnamed Actor>"));

	HWorld* GetWorld();

protected:
	void GuardedAddActor(AActor* pActor);

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(FCommandContext& CmdContext);

	virtual void Serialize( WriteContext& Value ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	std::vector<AActor*> m_Actors;
	CriticalSection m_ActorListGuard;
	HWorld* m_pOwningWorld;
	
};


//
// Inline function implementations
//

template <typename ActorType>
inline ActorType* HLevel::CreateActor(const HName& Name)
{
	HE_ASSERT( m_pOwningWorld != NULL ); // Cannot add an actor to a level with a null world!

	FActorInitArgs InitArgs{ m_pOwningWorld, Name, false };
	AActor* pNewActor = new ActorType( InitArgs);
	HE_ASSERT(pNewActor != NULL);

	GuardedAddActor(pNewActor);

	return SCast<ActorType*>(pNewActor);
}

inline void HLevel::GuardedAddActor(AActor* pActor)
{
	ScopedCriticalSection Guard( m_ActorListGuard );
	m_Actors.push_back(pActor);
}

inline HWorld* HLevel::GetWorld()
{
	return m_pOwningWorld;
}