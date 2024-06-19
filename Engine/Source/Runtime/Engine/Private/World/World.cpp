// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/World.h"

#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"
#include "Renderer/LightManager.h"
#include "Engine/Subsystem/PhysicsSubsystem.h"
#include "GameFramework/Actor/ACharacter.h"
#include "GameFramework/Actor/ADebugPawn.h"
#include "GameFramework/Actor/AThirdPersonCharacter.h"
#include "GameFramework/Actor/AFirstPersonCharacter.h"
#include "GameFramework/Components/HPlaneColliderComponent.h"
#include "GameFramework/Components/HSphereColliderComponent.h"
#include "GameFramework/Components/HCubeColliderComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"
#include "GameFramework/Components/HCameraComponent.h"

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

void HWorld::Initialize( const FPath& LevelURL )
{
	m_LevelFilepath = LevelURL;

	RegisterScenes();

	bool UseFirstPersonCharacter = false;

	FActorInitArgs PlrCharacterInitArgs{ this, TEXT( "Player Character" ), true };
	if (UseFirstPersonCharacter)
		m_pPlayerCharacter = new AFirstPersonCharacter( PlrCharacterInitArgs );
	else
		m_pPlayerCharacter = new AThirdPersonCharacter( PlrCharacterInitArgs );
	m_Level.GuardedAddActor( m_pPlayerCharacter );

	HCapsuleColliderComponent* pCapsule = m_pPlayerCharacter->GetComponent<HCapsuleColliderComponent>();
	pCapsule->SetPosition( m_pPlayerCharacter->GetRootComponent()->GetPosition() + FVector3(0.f, pCapsule->GetHeight(), 0.f));
	pCapsule->SetRotation( 0.f, 0.f, Math::DegreesToRadians(90.f) );
	
	rapidjson::Document WorldJsonDoc;
	FileRef WorldJsonSource( LevelURL.GetFullPath(), FUM_Read);
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
		HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName().c_str() );

		// TEMP
		m_DebugUI.AddWidget( m_FPSCounter );
		m_FPSCounter.SetText( L"FPS: " );

		AddPanel( &m_DebugUI );
	}
	else
	{
		HE_ASSERT( false );
	}
}

void HWorld::Initialize()
{
	m_LevelFilepath.SetPath( "Default (Non-Load)" );
	SetObjectName( TEXT( "Default World" ) );
	RegisterScenes();


	HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName().c_str() );
}

void HWorld::Save()
{
	Serialize( m_LevelFilepath.GetFullPath() );
}

void HWorld::SetViewport( FViewportContext* pViewport )
{
	m_pRenderingViewport = pViewport;
	m_pRenderingViewport->SetWorld( this );
}

void HWorld::RegisterScenes()
{
	FPhysicsSubsystem& PhysicsSubsystem = GEngine->GetPhysicsSubsystem();
	FRenderingSubsystem& RenderingSubsystem = GEngine->GetRenderingSubsystem();
	
	m_PhysicsScene.Setup( PhysicsSubsystem.GetPhysicsContext() );
	PhysicsSubsystem.AddSceneForSimulation( m_PhysicsScene );

	RenderingSubsystem.PushUIPanelForRendering( m_DebugUI );
	RenderingSubsystem.PushSceneForRendering( m_Scene );
}

void HWorld::BeginPlay()
{
	SetCurrentSceneRenderCamera( m_pPlayerCharacter->GetCameraComponent() );

	m_Level.BeginPlay();
}

void HWorld::Tick( float DeltaTime )
{
	//m_PhysicsScene.WaittillSimulationFinished(); // Sync the physics thread.
	
	m_PhysicsScene.Tick( GEngine->GetDeltaTimeUnscaled(), GEngine->GetDeltaTimeScale());

	static float SecondTimer = 0.f;
	static float FPS = 0.f;
	SecondTimer += GEngine->GetDeltaTimeUnscaled();
	if (SecondTimer > 1.f)
	{
		WString Label = L"FPS: " + std::to_wstring( (int)FPS );
		Label += L" (" + std::to_wstring( GEngine->GetDeltaTimeUnscaled() );
		Label += +L" ms)";
		m_FPSCounter.SetText( Label );
		FPS = 0.f;
		SecondTimer = 0.f;
	}
	else
		FPS++;

	m_CameraManager.Tick( DeltaTime );

	if (GEngine->IsPlayingInEditor())
	{
		m_Level.Tick( DeltaTime );
	}

	//m_PhysicsScene.RequestTick();
}

void HWorld::Flush()
{
	if (m_Level.IsValid())
	{
		// Unregister the physics scene to stop simulation, but dont destroy 
		// it yet components need it to be able to realease resources.
		/*m_PhysicsScene.WaittillSimulationFinished();
		m_PhysicsScene.RequestSceneFlush();*/

		RemovePanel( &m_DebugUI );

		GEngine->GetPhysicsSubsystem().RemoveSceneFromSimulation( m_PhysicsScene );

		// Cleanup the rendering resources.
		HE_LOG( Log, TEXT( "Flushing world: %s" ), GetObjectName().c_str() );
		GCommandManager.IdleGpu();
		GLightManager.FlushLightCache();
		GEngine->GetRenderingSubsystem().RemoveSceneFromRendering( m_Scene );

		// Cleanup the level and destroy all actors and components.
		m_Level.Flush();

		// Cleanup the physics scene.
		m_PhysicsScene.Teardown();
	}
}

void HWorld::Render( FCommandContext& CmdContext )
{
	m_Level.Render( CmdContext );
}

void HWorld::Reload()
{
	Flush();
	Initialize( m_LevelFilepath.GetFullPath() );
}

void HWorld::ReloadAndBeginPlay()
{
	Reload();
	BeginPlay();
}

void HWorld::Serialize( const Char* Filename )
{
	HE_ASSERT( Filename != NULL ); // World has a reference to its filepath. No need to pass one in.

	rapidjson::StringBuffer StrBuffer;
	JsonUtility::WriteContext Writer( StrBuffer );

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
				std::vector<AActor*>& LevelActors = m_Level.GetAllActors();
				for (uint32 i = 0; i < LevelActors.size(); i++)
				{
					AActor& Actor = *LevelActors[i];
					
					Writer.Key( Actor.GetGuid().ToString().CStr() );
					Writer.StartArray();

					Writer.StartObject();
					if (HSceneComponent* pRootComponenet = Actor.GetRootComponent())
					{
						JsonUtility::WriteTransformValues( Writer, pRootComponenet->m_Transform );
					}
					Writer.EndObject();

					Writer.EndArray();
				}

				//m_Level.Serialize( Writer );
			}
			Writer.EndObject();

		}
		Writer.EndArray();
	}
	Writer.EndObject();

	FileRef OutFile( m_LevelFilepath.GetFullPath(), FUM_Write, CM_Text);
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

void HWorld::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( "Name" );
	Output.String( TCharToChar( GetObjectName() ) );
}

void HWorld::Deserialize( const JsonUtility::ReadContext& Value )
{
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

void HWorld::AddSphereColliderComponent( HSphereColliderComponent* pSphere, bool IsStatic, bool IsTrigger /*= false*/ )
{
	m_PhysicsScene.CreateSphere( 
		pSphere->GetWorldPosition(), 
		pSphere->GetRotation(), 
		(HSphereRigidBody&)pSphere->GetRigidBody(), 
		IsTrigger, 
		(PhysicsCallbackHandler*)pSphere,
		false,
		10.f,
		IsStatic, FG_WorldGeometry );
}

void HWorld::AddPlaneColliderComponent( HPlaneColliderComponent* pPlane, bool IsStatic, bool IsTrigger /*= false*/ )
{
	m_PhysicsScene.CreatePlane( 
		pPlane->GetWorldPosition(), 
		pPlane->GetRotation(), (HPlaneRigidBody&)
		pPlane->GetRigidBody(), 
		IsTrigger, 
		(PhysicsCallbackHandler*)pPlane,
		false,
		10.f,
		IsStatic, FG_WorldGeometry );
}

void HWorld::AddCubeColliderComponent( HCubeColliderComponent* pCube, bool IsStatic, bool IsTrigger /*= false*/ )
{
	m_PhysicsScene.CreateCube( 
		pCube->GetWorldPosition(), 
		pCube->GetRotation(), 
		(HCubeRigidBody&)pCube->GetRigidBody(), 
		IsTrigger, 
		(PhysicsCallbackHandler*)pCube,
		false,
		10.f,
		IsStatic, FG_WorldGeometry );
}

void HWorld::AddCapsuleColliderComponent( HCapsuleColliderComponent* pCapsule, bool IsStatic, bool IsTrigger /*= false*/ )
{
	m_PhysicsScene.CreateCapsule( 
		pCapsule->GetWorldPosition(), 
		pCapsule->GetRotation(),
		(HCapsuleRigidBody&)pCapsule->GetRigidBody(), 
		IsTrigger, 
		(PhysicsCallbackHandler*)pCapsule,
		false,
		10.f,
		IsStatic,
		FG_Player, FG_WorldGeometry );
}

void HWorld::RemoveColliderComponent( HColliderComponent* pCollider )
{
	m_PhysicsScene.RemoveActor( pCollider->GetRigidBody() );
}

bool HWorld::Raycast( const FVector3& Origin, const FVector3& UnitDirection, float Distance, FRaycastHitInfo* HitResults /*= nullptr*/, std::vector<HColliderComponent*>* IgnoreActors/* = nullptr*/ )
{

	/*FDebugLineRenderInfo LineInfo = {};
	LineInfo.Start = Origin;
	LineInfo.End = Origin + UnitDirection * Distance;
	LineInfo.Color = FColor::BlueOpaque;
	LineInfo.Lifetime = 20.f;
	DrawDebugLine( LineInfo );*/
	std::vector<HRigidBody*> IgnoreRBs;
	if (IgnoreActors)
	{
		for (size_t i = 0; i < IgnoreActors->size(); i++)
		{
			HColliderComponent* pCollider = (*IgnoreActors)[i];
			if (pCollider)
			{
				IgnoreRBs.push_back( &pCollider->GetRigidBody() );
			}
		}
	}

	bool Hit = m_PhysicsScene.RayCast( Origin, UnitDirection, Distance, HitResults, &IgnoreRBs );
	/*if (Hit)
	{
		FDebugLineRenderInfo HitLineInfo = {};
		HitLineInfo.Start = HitResults.HitPos;
		HitLineInfo.End = LineInfo.End;
		HitLineInfo.Color = FColor::RedOpaque;
		HitLineInfo.Lifetime = LineInfo.Lifetime;
		DrawDebugLine( HitLineInfo );
	}*/
	return Hit;
}

float HWorld::GetMouseMoveDeltaX()
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return m_pRenderingViewport->GetMouseMoveDeltaX();
}

float HWorld::GetMouseMoveDeltaY()
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return m_pRenderingViewport->GetMouseMoveDeltaY();
}

bool HWorld::IsPressed( DigitalInput Key )
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return m_pRenderingViewport->IsPressed( Key );
}

bool HWorld::IsFirstPressed( DigitalInput Key )
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return m_pRenderingViewport->IsFirstPressed( Key );
}

bool HWorld::IsReleased( DigitalInput Key )
{
	HE_ASSERT( m_pRenderingViewport!= nullptr );
	return m_pRenderingViewport->IsReleased( Key );
}

float HWorld::GetWindowWidth()
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return (float)m_pRenderingViewport->GetWindow().GetWidth();
}

float HWorld::GetWindowHeight()
{
	HE_ASSERT( m_pRenderingViewport != nullptr );
	return (float)m_pRenderingViewport->GetWindow().GetHeight();
}