// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HActorComponent.h"
#include "GameFramework/Components/HStaticMeshComponent.h"


 HCOMPONENT()
class HBillboardComponent : public HStaticMeshComponent
 {
	 friend class AActor;
	 using Super = HStaticMeshComponent;
 public:
	 HE_COMPONENT_GENERATED_BODY( HBillboardComponent )

	 virtual void BeginPlay() override;
	 virtual void Tick( float DeltaTime ) override;

	 virtual void OnCreate() override;
	 virtual void OnDestroy() override;

 protected:


 };
