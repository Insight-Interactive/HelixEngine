#pragma once

#include "Engine/Subsystem/SubsystemInterface.h"


/*
	Handles all physics interactions in the world.
*/
class FPhysicsSubsystem : public FSubsystemInterface
{
	friend class HEngine;
public:

protected:
	FPhysicsSubsystem();
	virtual ~FPhysicsSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual void RunAsync_Implementation() override;

private:

};
