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

	template <typename ActorType, typename ... InitArgs>
	ActorType* CreateActor(const HName& Name = TEXT(""), InitArgs ... args);

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

template <typename ActorType, typename ... InitArgs>
inline ActorType* HLevel::CreateActor(const HName& Name, InitArgs ... args)
{
	HE_ASSERT( m_pOwningWorld != NULL ); // Cannot add an actor to a level with a null world.

	AActor* pNewActor = new ActorType( m_pOwningWorld, Name, args...);
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