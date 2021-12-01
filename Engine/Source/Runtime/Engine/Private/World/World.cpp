// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/World.h"

#include "World/Level.h"
#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"
#include "Renderer/LightManager.h"
#include "GameFramework/Actor/APlayerCharacter.h"


HWorld::HWorld()
	: m_CameraManager(this)
	, m_pLevel(NULL)
	, m_Scene(this)
	, m_pPlayerCharacter(NULL)
	, m_RenderingCamera(NULL)
	, m_pRenderingViewport(NULL)
{
}

HWorld::~HWorld()
{
	Flush();
}

void HWorld::Initialize(const Char* LevelURL )
{
	m_Filepath = LevelURL;


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
		m_pLevel = new HLevel( this );
		m_pLevel->Deserialize( WorldActors );
	}
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

void HWorld::BeginPlay()
{
	FActorInitArgs InitArgs{ this, TEXT( "Player Character" ) };
	m_pPlayerCharacter = new APlayerCharacter( InitArgs );
	SetCurrentSceneRenderCamera( m_pPlayerCharacter->GetCameraComponent() );
	AddPlayerCharacterRef( m_pPlayerCharacter );

	m_pLevel->BeginPlay();
	m_pPlayerCharacter->BeginPlay();
}

void HWorld::Tick(float DeltaTime)
{
	m_CameraManager.Tick( DeltaTime );

	if (GEngine->IsPlayingInEditor())
	{
		m_pPlayerCharacter->Tick( DeltaTime );
		m_pLevel->Tick( DeltaTime );
	}
}

void HWorld::Flush()
{
	HE_LOG( Log, TEXT( "Flushing world: %s" ), GetObjectName().c_str() );
	GCommandManager.IdleGpu();
	if (m_pLevel != NULL)
	{
		m_pLevel->Flush();
		HE_SAFE_DELETE_PTR( m_pLevel );
	}
	m_PlayerCharacterRefs.clear();
	HE_SAFE_DELETE_PTR( m_pPlayerCharacter );
}

void HWorld::Render(FCommandContext& CmdContext)
{
	if(m_pPlayerCharacter != NULL)
		m_pPlayerCharacter->Render( CmdContext );

	m_pLevel->Render(CmdContext);
}

void HWorld::Reload()
{
	Flush();
	GLightManager.FlushLightCache();
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
				m_pLevel->Serialize( Writer );
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
