// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/World.h"

#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"
#include "Renderer/LightManager.h"
#include "GameFramework/Actor/APlayerCharacter.h"
#include "Engine/Subsystem/PhysicsSubsystem.h"
#include "GameFramework/Components/HPlaneColliderComponent.h"
#include "GameFramework/Components/HSphereColliderComponent.h"
#include "GameFramework/Components/HCubeColliderComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


HWorld::HWorld()
	: m_CameraManager( this )
	, m_Scene( this )
	, m_Level( this )
	, m_pPlayerCharacter( NULL )
	, m_RenderingCamera( NULL )
	, m_pRenderingViewport( NULL )
{
}

HWorld::~HWorld()
{
	Flush();
}

void HWorld::Initialize( const Char* LevelURL )
{
	m_Filepath = LevelURL;

	RegisterScenes();

	rapidjson::Document WorldJsonDoc;
	FileRef WorldJsonSource( LevelURL, FUM_Read );
	HE_ASSERT( WorldJsonSource->IsOpen() );
	JsonUtility::LoadDocument( WorldJsonSource, WorldJsonDoc );
	if (WorldJsonDoc.IsObject())
	{
		const rapidjson::Value& World = WorldJsonDoc["World"];
		enum
		{
			kSettings = 0,
			kActors = 1,
		};
		// Load the world's settings
		const rapidjson::Value& WorldSettings = World[kSettings];
		Deserialize( WorldSettings );

		// Load the worlds actors.
		const rapidjson::Value& WorldActors = World[kActors];
		m_Level.Deserialize( WorldActors );
	}

	HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName().c_str() );
}

void HWorld::Initialize()
{
	m_Filepath = "Default (Non-Load)";
	SetObjectName( TEXT( "Default World" ) );
	RegisterScenes();

	HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName().c_str() );
}

float HWorld::GetDeltaTime() const
{
	return (float)GEngine->GetDeltaTime();
}

void HWorld::SetViewport( FViewportContext* pViewport )
{
	m_pRenderingViewport = pViewport;
	m_pRenderingViewport->SetWorld( this );
}

void HWorld::RegisterScenes()
{
	m_PhysicsScene.Setup( GEngine->GetPhysicsSubsystem().GetPhysicsContext() );

	GEngine->GetRenderingSubsystem().PushSceneForRendering( m_Scene );
	GEngine->GetPhysicsSubsystem().AddSceneForSimulation( m_PhysicsScene );
}

void HWorld::BeginPlay()
{
	FActorInitArgs InitArgs{ this, TEXT( "Player Character" ), true };
	m_pPlayerCharacter = new APlayerCharacter( InitArgs );
	SetCurrentSceneRenderCamera( m_pPlayerCharacter->GetCameraComponent() );
	AddPlayerCharacterRef( m_pPlayerCharacter );

	m_Level.BeginPlay();
	m_pPlayerCharacter->BeginPlay();
}

void HWorld::Tick( float DeltaTime )
{
	m_PhysicsScene.WaittillSimulationFinished(); // Sync the physics thread.
	
	m_CameraManager.Tick( DeltaTime );

	if (GEngine->IsPlayingInEditor())
	{
		m_pPlayerCharacter->Tick( DeltaTime );
		m_Level.Tick( DeltaTime );
	}

	m_PhysicsScene.RequestTick();
}

void HWorld::Flush()
{
	if (m_Level.IsValid())
	{
		// Unregister the physics scene to stop simulation, but dont destroy 
		// it yet components need it to be able to realease resources.
		m_PhysicsScene.WaittillSimulationFinished();
		m_PhysicsScene.RequestSceneFlush();

		GEngine->GetPhysicsSubsystem().RemoveSceneFromSimulation( m_PhysicsScene );

		// Cleanup the rendering resources.
		HE_LOG( Log, TEXT( "Flushing world: %s" ), GetObjectName().c_str() );
		GCommandManager.IdleGpu();
		GLightManager.FlushLightCache();
		m_Scene.WaitForRenderingFinished();
		GEngine->GetRenderingSubsystem().RemoveSceneFromRendering( m_Scene );

		// Cleanup the level and destroy all actors and components.
		m_Level.Flush();
		m_PlayerCharacterRefs.clear();
		HE_SAFE_DELETE_PTR( m_pPlayerCharacter );

		// Cleanup the physics scene.
		m_PhysicsScene.Teardown();
	}
}

void HWorld::Render( FCommandContext& CmdContext )
{
	if (m_pPlayerCharacter != NULL)
		m_pPlayerCharacter->Render( CmdContext );

	m_Level.Render( CmdContext );
}

void HWorld::Reload()
{
	Flush();
	Initialize( m_Filepath.c_str() );
}

void HWorld::ReloadAndBeginPlay()
{
	Reload();
	BeginPlay();
}

void HWorld::Serialize( const Char* Filename )
{
	HE_ASSERT( Filename == NULL ); // World has a reference to its filepath. No need to pass one in.

	rapidjson::StringBuffer StrBuffer;
	WriteContext Writer( StrBuffer );

	Writer.StartObject();
	{
		Writer.Key( "World" );
		Writer.StartArray();
		{
			// Serialize the world settings.
			Writer.StartObject();
			{
				Serialize( Writer );
			}
			Writer.EndObject();

			// Serialize the level.
			Writer.StartObject();
			{
				m_Level.Serialize( Writer );
			}
			Writer.EndObject();

		}
		Writer.EndArray();
	}
	Writer.EndObject();

	FileRef OutFile( m_Filepath, FUM_Write, CM_Text );
	HE_ASSERT( OutFile->IsOpen() );
	if (OutFile->IsOpen())
	{
		if (!OutFile->WriteData( (void*)StrBuffer.GetString(), StrBuffer.GetSize(), 1 ))
		{
			HE_LOG( Error, TEXT( "Failed to serialize database %s" ), TCharToChar( GetObjectName() ) );
			HE_ASSERT( false );
		}
	}
}

void HWorld::Serialize( WriteContext& Output )
{
	Output.Key( "Name" );
	Output.String( TCharToChar( GetObjectName() ) );

	Output.Key( "TickInterval" );
	Output.Double( 1.0 );
}

void HWorld::Deserialize( const ReadContext& Value )
{
	float TickInterval = 0.f;
	JsonUtility::GetFloat( Value, "TickInterval", TickInterval );

	Char WorldName[64];
	JsonUtility::GetString( Value, "Name", WorldName, sizeof( WorldName ) );
	Super::SetObjectName( CharToTChar( WorldName ) );
}

void HWorld::DrawDebugLine( const FDebugLineRenderInfo& LineInfo )
{
	GetOwningViewport()->GetSceneRenderer().DrawDebugLine( LineInfo );
}

void HWorld::PausePhysics()
{
	m_PhysicsScene.RequestPauseSimulation();
}

void HWorld::UnPausePhysics()
{
	m_PhysicsScene.RequestUnPauseSimulation();
}

void HWorld::AddSphereColliderComponent( HSphereColliderComponent* pSphere, bool StartDisabled )
{
	HColliderComponent* pCollider = (HColliderComponent*)pSphere;

	PhysicsScene::SphereActorAddDesc InitDesc
	{
		pSphere,
		pCollider->GetAbsoluteWorldPosition(),
		(SphereRigidBody&)pSphere->GetRigidBody(),
		StartDisabled
	};
	m_PhysicsScene.RequestSphereActorAdd( InitDesc );
}

void HWorld::AddPlaneColliderComponent( HPlaneColliderComponent* pPlane, bool StartDisabled )
{
	HColliderComponent* pCollider = (HColliderComponent*)pPlane;

	PhysicsScene::PlaneActorAddDesc InitDesc
	{
		pPlane,
		pCollider->GetAbsoluteWorldPosition(),
		(PlaneRigidBody&)pPlane->GetRigidBody(),
		StartDisabled
	};
	m_PhysicsScene.RequestPlaneActorAdd( InitDesc );
}

void HWorld::AddCubeColliderComponent( HCubeColliderComponent* pCube, bool StartDisabled )
{
	HColliderComponent* pCollider = (HColliderComponent*)pCube;

	PhysicsScene::CubeActorAddDesc InitDesc
	{
		pCube,
		pCollider->GetPosition(),
		(CubeRigidBody&)pCube->GetRigidBody(),
		StartDisabled
	};
	m_PhysicsScene.RequestCubeActorAdd( InitDesc );
}

void HWorld::AddCapsuleColliderComponent( HCapsuleColliderComponent* pCapsule, bool StartDisabled )
{
	HColliderComponent* pCollider = (HColliderComponent*)pCapsule;

	PhysicsScene::CapsuleActorAddDesc InitDesc
	{
		pCapsule,
		pCollider->GetPosition(),
		(CapsuleRigidBody&)pCapsule->GetRigidBody(),
		StartDisabled
	};
	m_PhysicsScene.RequestCapsuleActorAdd( InitDesc );
}

void HWorld::RemoveColliderComponent( HColliderComponent* pCollider )
{
	m_PhysicsScene.RequestActorRemove( pCollider->GetRigidBody() );
}
