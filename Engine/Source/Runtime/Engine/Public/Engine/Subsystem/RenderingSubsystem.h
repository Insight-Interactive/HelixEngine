#pragma once

#include "Engine/Subsystem/SubsystemInterface.h"

#include "RenderContext.h"
#include "CriticalSection.h"

class HScene;

/*
	Asynchronously renders multiple scenes.
*/
class FRenderingSubsystem : public FSubsystemInterface
{
	friend class HEngine;
public:
	void PushSceneForRendering( HScene& Scene );
	bool RemoveSceneFromRendering( HScene& Scene );
	
protected:
	FRenderingSubsystem();
	virtual ~FRenderingSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual void RunAsync_Implementation() override;

private:
	CriticalSection m_SceneRenderMutex;
	std::vector<HScene*> m_Scenes;

	FRenderContext m_RenderContext;

};


//
// Inline function implementations
//

FORCEINLINE void FRenderingSubsystem::PushSceneForRendering( HScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneRenderMutex );
	m_Scenes.push_back( &Scene );
}

FORCEINLINE bool FRenderingSubsystem::RemoveSceneFromRendering( HScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneRenderMutex );
	auto Iter = std::find( m_Scenes.begin(), m_Scenes.end(), &Scene );
	if (Iter != m_Scenes.end())
	{
		m_Scenes.erase( Iter );
		return true;
	}
	HE_LOG( Warning, TEXT( "Trying to remove a scene from rendering subsystem that does not exist!" ) );
	return false;
}
