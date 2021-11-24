// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HBillboardComponent.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "Renderer/GeometryGenerator.h"
#include "World/World.h"


HBillboardComponent::HBillboardComponent( const HName& Name )
	: HStaticMeshComponent( Name )
{
}

HBillboardComponent::~HBillboardComponent()
{
}

void HBillboardComponent::BeginPlay()
{
	SetMesh( GeometryGenerator::GenerateScreenAlignedQuadMesh() );	
}

void HBillboardComponent::Tick( float DeltaTime )
{
	SetRotation( GetWorld()->GetCurrentSceneRenderCamera()->GetTransform().GetRotation() );
}

void HBillboardComponent::OnCreate()
{
}

void HBillboardComponent::OnDestroy()
{
}
