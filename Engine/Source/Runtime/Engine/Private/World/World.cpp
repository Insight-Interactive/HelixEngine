// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/World.h"

#include "Engine/Engine.h"
#include "Graphics/LightManager.h"
#include "Engine/ViewportContext.h"
#include "GameFramework/Actor/AThirdPersonCharacter.h"
#include "GameFramework/Actor/AFirstPersonCharacter.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"


HWorld::HWorld()
	: HObject( "World" )
	, m_CameraManager( this )
	, m_Scene( this )
	, m_Level( this )
	, m_PlayerCharacter( NULL )
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

	m_PlayerCharacter = m_Level.CreateActor<AThirdPersonCharacter>( "PlayerCharacter" );

	rapidjson::Document WorldJsonDoc;
	FileRef WorldJsonSource( LevelURL.GetFullPath(), FUM_Read);
	HE_ASSERT( WorldJsonSource->IsOpen() );
	JsonUtility::LoadDocument( WorldJsonSource, WorldJsonDoc );
	if (WorldJsonDoc.IsObject())
	{
		// Load the world's settings
		Deserialize( WorldJsonDoc );

		// Load the worlds actors.
		const rapidjson::Value& Actors = WorldJsonDoc["Actors"];
		HE_ASSERT( Actors.IsArray() );
		m_Level.Deserialize( Actors );
		HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName() );

		// Load static placed lights
		/*const rapidjson::Value& Lights = WorldJsonDoc["Lights"];
		StringHashValue PointLightHash = StringHash( "PointLight" );
		StringHashValue SpotLightHash = StringHash( "SpotLight" );
		for (auto Iter = Lights.MemberBegin(); Iter != Lights.MemberEnd(); Iter++)
		{
			StringHashValue LightTypeHash = StringHash(Iter->name.GetString());
			if (PointLightHash == LightTypeHash)
			{
				PointLightData* pData = nullptr;
				GLightManager.AllocatePointLightData( m_TestPointLight, &pData );
				HE_ASSERT( pData!= nullptr );

				FTransform Transform;
				JsonUtility::GetTransform( Lights, Iter->name.GetString(), Transform);
				pData->Position = Transform.GetPosition();

				JsonUtility::GetFloat( Iter->value[1], "R", pData->Color.x );
				JsonUtility::GetFloat( Iter->value[1], "G", pData->Color.y );
				JsonUtility::GetFloat( Iter->value[1], "B", pData->Color.z );
				JsonUtility::GetFloat( Iter->value[1], "Brightness", pData->Brightness );
				JsonUtility::GetFloat( Iter->value[1], "Radius", pData->Radius );
			}
			else if (SpotLightHash == LightTypeHash)
			{

			}
			else
			{
				HE_ASSERT( false );
			}
		}*/

		DirectionalLightCBData* Sun = GLightManager.GetWordSunDirectionalLight();
		Sun->Direction = FVector4( -0.2f, -1.0f, -0.3f, 0.f );
		Sun->Color = FVector4::One * 255.f;
		Sun->Brightness = 1.f;

		m_FPSCounter.SetPosition( FVector2( 0.01f, 0.01f ) );
		m_DebugUI.AddWidget( m_FPSCounter );

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
	SetObjectName( "Default World" );


	HE_LOG( Log, TEXT( "Level loaded with name: %s" ), GetObjectName() );
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

void HWorld::BeginPlay()
{
	SetCurrentSceneRenderCamera( m_PlayerCharacter->GetCameraComponent() );

	m_Level.BeginPlay();
}

void HWorld::Tick( float DeltaTime )
{

	FVector3 PathStart( -4.5f, 0.f, 4.5f );
	FVector3 PathEnd( 0.f, 1.f, -3.0f );
	//FVector3 PathStart( 7.8f, 113.7f, 419.0f );
	//FVector3 PathEnd( 100.0f, 6.7f, -50.0f );
	m_NavMesh.FindPath( PathStart, PathEnd );

	/*for (uint32 i = 0; i < m_NavMesh.m_nstraightPath; i+=6)
	{
		FVector3 LineStart( i, i + 1, i + 2 );
	}*/

	m_CameraManager.Tick( DeltaTime );

	if (GEngine->IsPlayingInEditor())
	{
		m_Level.Tick( DeltaTime );
	}
}

void HWorld::Flush()
{
	if (m_Level.IsValid())
	{
		RemovePanel( &m_DebugUI );

		// Cleanup the rendering resources.
		HE_LOG( Log, TEXT( "Flushing world: %s" ), GetObjectName() );
		GCommandManager.IdleGpu();
		GLightManager.FlushLightCache();

		m_Scene.UnloadWorldGeo();
		GStaticGeometryManager.FlushCache();

		// Cleanup the level and destroy all actors and components.
		m_Level.Flush();

		m_NavMesh.UnInit();
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
		Writer.Key( "WorldName" );
		Writer.String( GetObjectName() );

		Writer.Key( "Actors" );
		Writer.StartArray();
		{

			// Serialize the level.
			std::vector<AActor*>& LevelActors = m_Level.GetAllActors();
			for (uint32 i = 0; i < LevelActors.size(); i++)
			{
				AActor& Actor = *LevelActors[i];
					
				Writer.Key( Actor.GetGuid().ToString().CStr() );
				Writer.StartArray();

				Writer.StartObject();
				FTransform& Transform = Actor.GetTransform();
				JsonUtility::WriteTransformValues( Writer, Transform );
				Writer.EndObject();

				Writer.StartObject();
				{
					Writer.String( "InstanceOverrides" );
					Writer.StartArray();

					Writer.EndArray();
				}
				Writer.EndObject();

				Writer.EndArray();
			}

			//m_Level.Serialize( Writer );
			Writer.EndObject();

		}
		Writer.EndArray();

		Writer.Key( "Lights" );
		Writer.StartArray();
		{

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
			HE_LOG( Error, TEXT( "Failed to serialize database %s" ), GetObjectName() );
			HE_ASSERT( false );
		}
	}
}

void HWorld::Deserialize( const JsonUtility::ReadContext& Value )
{
	JsonUtility::GetString( Value, "WorldName", m_Name, sizeof( m_Name ) );

	char StringBuff[64];
	if (JsonUtility::GetString( Value, "WorldGeo", StringBuff, sizeof( StringBuff ) ))
	{
		FPath WorldGeoPath;
		sprintf_s( WorldGeoPath.m_Path, "%s%s", FGameProject::GetInstance()->GetContentFolder(), StringBuff );
		m_Scene.LoadWorldGeo( WorldGeoPath );
	}
	else
	{
		HE_LOG( Warning, TEXT( "Level has no geo associated with it! Relying on actor collision, this is not recomended." ) );
	}

	ZeroMemory( StringBuff, sizeof( StringBuff ) );
	if (JsonUtility::GetString( Value, "NavMesh", StringBuff, sizeof( StringBuff ) ))
	{
		FPath NavPath;
		sprintf_s( NavPath.m_Path, "%s%s", FGameProject::GetInstance()->GetContentFolder(), StringBuff );
		m_NavMesh.Init( NavPath );
	}


	FTransform PlayerStart;
	JsonUtility::GetTransform( Value, "PlayerStart", PlayerStart );
	m_PlayerCharacter->Teleport( PlayerStart.GetPosition() );
	HCameraBoomComponent* CameraBoom = m_PlayerCharacter->GetComponent<HCameraBoomComponent>();
	CameraBoom->SetBoomRotationAngles( PlayerStart.GetEulerRotation() );
}

void HWorld::DrawDebugLine( const FDebugLineRenderInfo& LineInfo )
{
	GetOwningViewport()->GetSceneRenderer().DrawDebugLine( LineInfo );
}

bool HWorld::Raycast( const FVector3& Origin, const FVector3& UnitDirection, float Distance, FRaycastHitInfo* HitResults /*= nullptr*/, std::vector<HColliderComponent*>* IgnoreActors/* = nullptr*/ )
{
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

	return Physics::RayCast( Origin, UnitDirection, Distance, HitResults, &IgnoreRBs );
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
