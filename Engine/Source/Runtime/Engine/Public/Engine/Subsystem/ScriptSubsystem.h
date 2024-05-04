// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Subsystem/SubsystemInterface.h"

class FScriptSubsystem : public FSubsystemInterface
{
public:
	FScriptSubsystem();
	virtual ~FScriptSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual void RunAsync_Implementation() override;

private:

};
