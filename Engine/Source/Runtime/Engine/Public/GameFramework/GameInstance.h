// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "TSingleton.h"

#include "Engine/Engine.h"


class ACharacter;
class EngineBeginPlayEvent;

class HGameInstance : public TSingleton<HGameInstance>
{
	friend class HEngine;
public:
	HGameInstance();
	virtual ~HGameInstance();

	virtual void OnGameSetFocus();
	virtual void OnGameLostFocus();
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );

	HWorld* GetWorld();

protected:
	void SetWorld( HWorld* pWorld );
	
	void OnEvent( EngineEvent& e );
	bool OnEnginePostStartup( EngineBeginPlayEvent& e );

	void ToggleMenu();

protected:
	HWorld* m_pWorld;

};


// Inline function imlpementations
//

FORCEINLINE void HGameInstance::SetWorld( HWorld* pWorld )
{
	m_pWorld = pWorld;
}

FORCEINLINE HWorld* HGameInstance::GetWorld()
{
	return m_pWorld;
}