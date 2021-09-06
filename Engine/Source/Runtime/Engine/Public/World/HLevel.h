#pragma once

#include "CriticalSection.h"

class AActor;
class HWorld;
class ICommandContext;


class HLevel
{
	friend class HWorld;
public:
	HLevel(HWorld* pOwner);
	~HLevel();

	void LoadFromFile(const Char* FileName);
	void Flush();

	template <typename ActorType, typename ... InitArgs>
	ActorType* CreateActor(InitArgs ... args);

	HWorld* GetWorld();

protected:
	void GuardedAddActor(AActor* pActor);

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(ICommandContext& CmdContext);
	
protected:
	std::vector<AActor*> m_Actors;
	CriticalSection m_ActorListGuard;
	HWorld* m_pWorld;
};


//
// Inline function implementations
//

template <typename ActorType, typename ... InitArgs>
inline ActorType* HLevel::CreateActor(InitArgs ... args)
{
	HE_ASSERT( m_pWorld != NULL ); // Cannot add an actor to a level with a null world.

	AActor* pNewActor = new ActorType( m_pWorld, args...);
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
	return m_pWorld;
}