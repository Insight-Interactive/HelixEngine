// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/HLevel.h"

#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Actor/APlayerCharacter.h"
#include "RendererCore.h"
#include "ModelManager.h"
#include "World/HWorld.h"
#include "FileSystem.h"
#include "GameFramework/Actor/Components/HStaticMeshComponent.h"
#include "GameFramework/Actor/Components/HPointLightComponent.h"

#include "RapidXML/rapidxml.hpp"


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
	StaticMeshGeometryRef CubeMesh = GStaticGeometryManager.LoadHAssetMeshFromFile("Content/Models/SM_Cube.hasset");
	MaterialRef RustedMetalMaterial = GMaterialManager.LoadMaterialFromFile("M_RustedMetal.hmat");
	TextureRef AlbedoTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Albedo.dds", DT_Magenta2D, false);
	TextureRef NormalTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Normal.dds", DT_Magenta2D, false);
	
	RustedMetalMaterial->SetAlbedoTexture(AlbedoTexture);
	RustedMetalMaterial->SetNormalTexture(NormalTexture);

	AActor* CubeActor = CreateActor<AActor>();
	HStaticMeshComponent* SMSubeActorCube = CubeActor->AddComponent<HStaticMeshComponent>(TEXT("Cube Mesh"));
	SMSubeActorCube->GetTransform().SetScale( 10.f, 10.f, 10.f );
	SMSubeActorCube->SetMaterial(RustedMetalMaterial);
	SMSubeActorCube->SetMesh(CubeMesh);
	HPointLightComponent* pPointLight = CubeActor->AddComponent<HPointLightComponent>( TEXT( "Point Light" ) );

	APlayerCharacter* pPlayer = CreateActor<APlayerCharacter>();
	m_pOwner->SetCurrentSceneRenderCamera(pPlayer->GetCameraComponent());
	GetWorld()->AddPlayerCharacterRef( pPlayer );

	return;
	FileRef File( "Content/Actors/ThirdPersonPlayerCharacter.xml", FUM_Read, CM_Text );

	rapidxml::xml_document<char> doc;
	doc.parse<0>( (char*)File->Data() );

	enum AssetType
	{
		Actor,
		Material,
	};

	enum ActorCategory
	{
		UserDefined,
		Base,
		PlayerCharacter,
		PlayerController,
		Pawn,
	};

	rapidxml::xml_node<>* pAssetNode = doc.first_node( "Asset" );
	char* val = pAssetNode->value();
	// Type
	rapidxml::xml_attribute<char>* pType = pAssetNode->first_attribute();
	char* TypeName = pType->name();
	char* TypeValue = pType->value();
	AssetType AssetCategory = (AssetType)(TypeValue[0] - 48);
	// Category
	rapidxml::xml_attribute<char>* pCategory = pType->next_attribute();
	char* pCategoryName = pCategory->name();
	char* pCategoryValue = pCategory->value();
	ActorCategory ActorCat = (ActorCategory)(pCategoryValue[0] - 48);

	rapidxml::xml_node<>* pDisplayNameNode = pAssetNode->first_node( "DisplayName" );
	char* DisplayName = pDisplayNameNode->value();

	rapidxml::xml_node<>* pComponentsNode = pDisplayNameNode->next_sibling();
	rapidxml::xml_node<>* pSMNode = pComponentsNode->first_node( "StaticMeshComponent" );
	rapidxml::xml_attribute<char>* pSMFileInfo = pSMNode->first_attribute();
	char* SMFileName = pSMFileInfo->name();
	char* SMFilepath = pSMFileInfo->value();

	AActor* pNewActor = CreateActor<AActor>();
	
}

void HLevel::BeginPlay()
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->BeginPlay();
	}
}

void HLevel::Tick(float DeltaTime)
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Tick(DeltaTime);
	}
}

void HLevel::Flush()
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->RemoveAllComponents();
		delete m_Actors[i];
	}
}

void HLevel::Render(ICommandContext& CmdContext)
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Render(CmdContext);
	}
}
