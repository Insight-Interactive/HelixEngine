#pragma once

#include "Engine/Subsystem/SubsystemInterface.h"


class HScene;

/*
	Asynchronously renders multiple scenes.
*/
class FRenderingSubsystem : public FSubsystemInterface
{
	friend class HEngine;
public:
	
protected:
	FRenderingSubsystem();
	virtual ~FRenderingSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	void PushSceneForRendering( HScene& Scene );

private:
	virtual void RunAsync_Implementation() override;

private:
	std::vector<HScene*> m_Scenes;

};


//
// Inline function implementations
//

FORCEINLINE void FRenderingSubsystem::PushSceneForRendering( HScene& Scene )
{
	m_Scenes.push_back( &Scene );
}