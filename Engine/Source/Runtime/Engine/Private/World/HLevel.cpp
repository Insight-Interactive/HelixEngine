// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/HLevel.h"

#include "Actor/AActor.h"
#include "Actor/APlayerCharacter.h"
#include "RendererCore.h"
#include "ModelManager.h"
#include "World/HWorld.h"

#include "Actor/Components/HStaticMeshComponent.h"


HLevel::HLevel(HWorld* pOwner)
	: m_pOwner(pOwner)
{
}

HLevel::~HLevel()
{
}

void HLevel::LoadFromFile(const Char* FileName)
{
	// Load Resources
	//
	GStaticGeometryManager.LoadFBXFromFile("Content/Models/Cube.fbx");
	GMaterialManager.LoadMaterialFromFile("M_RustedMetal.hmat");
	TextureRef AlbedoTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Albedo.dds", DT_Magenta2D, false);
	TextureRef NormalTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Normal.dds", DT_Magenta2D, false);

	// Get References
	//
	StaticMeshGeometryRef CubeMesh = GStaticGeometryManager.GetStaticMeshByName("Cube");
	MaterialRef RustedMetalMaterial = GMaterialManager.GetMaterialByName("M_RustedMetal");
	
	RustedMetalMaterial->SetAlbedoTexture(AlbedoTexture);
	RustedMetalMaterial->SetNormalTexture(NormalTexture);

	AActor* CubeActor = CreateActor<AActor>();
	HStaticMeshComponent* SMSubeActorCube = CubeActor->AddComponent<HStaticMeshComponent>(TEXT("Cube Mesh"));
	SMSubeActorCube->SetMaterial(RustedMetalMaterial);
	SMSubeActorCube->SetMesh(CubeMesh);

	APlayerCharacter* pPlayer = CreateActor<APlayerCharacter>();
	m_pOwner->SetCurrentSceneRenderCamera(pPlayer->GetCameraComponent());

}

void HLevel::BeginPlay()
{
	for (size_t i = 0; i < m_Actors.Size(); ++i)
	{
		m_Actors[i]->BeginPlay();
	}
}

void HLevel::Tick(float DeltaTime)
{
	for (size_t i = 0; i < m_Actors.Size(); ++i)
	{
		m_Actors[i]->Tick(DeltaTime);
	}
}

void HLevel::Flush()
{
	for (size_t i = 0; i < m_Actors.Size(); ++i)
	{
		m_Actors[i]->RemoveAllComponents();
		delete m_Actors[i];
	}
}

void HLevel::Render(ICommandContext& CmdContext)
{
	for (size_t i = 0; i < m_Actors.Size(); ++i)
	{
		m_Actors[i]->Render(CmdContext);
	}
}
