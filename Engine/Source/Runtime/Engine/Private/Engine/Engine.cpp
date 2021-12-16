// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Engine.h"

#include "System.h"
#include "ThreadPool.h"
#include "FileSystem.h"
#include "GameFramework/Game.h"
#include "GameFramework/GameInstance.h"

#include "GpuResource.h"
#include "CommandContext.h"
#include "CommandManager.h"
#include "RendererInitializer.h"
#include "Engine/SplashScreen.h"
#include "Engine/Event/EngineEvent.h"


Logger GEngineLogger;
HEngine* GEngine = nullptr;
ThreadPool* GThreadPool = nullptr;
HGameInstance* GGameInstance = nullptr;

// Static function declarations
//
static void SplashMain( void* pUserData );

//
// Engine
//

HEngine::HEngine( FCommandLine& CmdLine )
	: m_IsInitialized( false )
	, m_IsEditorPresent( CmdLine[L"-launchcfg"] == L"LaunchEditor" )
	, m_IsPlayingInEditor( !m_IsEditorPresent )
	, m_AppSeconds( 0.0 )
{
}

HEngine::~HEngine()
{
}

void HEngine::EngineMain()
{
	if (IsInitialized())
	{
		HE_LOG( Warning, TEXT( "Trying to call Engine::EngineMain() on an engine instance that is already initialized!" ) );
		HE_DEBUG_BREAK();
	}
	// Startup
	PreStartup();
	Startup();
	PostStartup();

	// Update
	Update();

	// Shutdown
	PreShutdown();
	Shutdown();
	PostShutdown();
}

void HEngine::PreStartup()
{
	CreateLogger( GEngineLogger, "Helix Engine" );
	HE_LOG( Log, TEXT( "Beginning engine pre-startup." ) );

	System::InitializePlatform();

	// Initialize the thread pool.
	GThreadPool = new ThreadPool( System::GetProcessorCount(), NULL );
	GThreadPool->SpawnWorkers();

	// Initialize the filesystem.
	FileSystem::Init();
#if HE_DEBUG_FILESYSTEM
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( sizeof( Path ), Path );
	HE_LOG( Log, TEXT( "FileSystem initialized with working directory: %s" ), Path );
#endif

	// Initialize the renderer.
	FRendererInitializer::InitializeContext( m_RenderContext );

	// Initialize Subsystems
	m_ReneringSubsystem.Initialize();
	m_PhysicsSubsystem.Initialize();

	m_ReneringSubsystem.RunAsync();
	m_PhysicsSubsystem.RunAsync();

	HE_LOG( Log, TEXT( "Engine pre-startup complete." ) );
}

void HEngine::Startup()
{
	HE_LOG( Log, TEXT( "Starting up engine." ) );

	// TODO: Make this dynamic
	const Char* GameProjectDirectory =
#if HE_STANDALONE
		"Data\\Game.hproject";
#else
		"C:\\Dev\\InsightInteractive\\HelixProjects\\Game\\Game.hproject";
#endif
	FGameProject::GetInstance()->Startup( GameProjectDirectory );

	// Initialize the application context.
	FApp::GetInstance()->Startup();

	// Initialize the game.
	//GGameInstance = MakeGameInstance();
	GGameInstance = new HGameInstance();

	FAssetDatabase::Initialize();

	// Create and initialize the main client window.
	FWindow::Description ClientDesc = {};
	ClientDesc.bHasTitleBar		= true;
	ClientDesc.bShowImmediate	= false;
	ClientDesc.Resolution		= GCommonResolutions[k1080p];
#if HE_WITH_EDITOR
	HName EngineTitle			=  TEXT( "Helix Editor" ) ;
	EngineTitle					+= TEXT( " (" ) + FGameProject::GetInstance()->GetProjectName() + TEXT( ")" );
	EngineTitle					+= TEXT( " [" HE_PLATFORM_STRING " - " HE_CONFIG_STRING "]" );
	if (!GetIsEditorPresent())
		EngineTitle += TEXT( " (Standalone)" );

	ClientDesc.Title			=  EngineTitle.c_str();
#else
	ClientDesc.Title			= FApp::GetInstance()->GetName();
#endif
	ClientDesc.bAllowDropFiles	= GetIsEditorPresent();
	m_MainViewPort.Initialize( ClientDesc );
	m_MainViewPort.GetWindow().AddListener( this, &HEngine::OnEvent );


#if HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH
	// Create the splash screen to serve as a loading indicator.
	GThreadPool->Kick( SplashMain, NULL );
	//std::this_thread::sleep_for(std::chrono::seconds(2)); // Uncomment to delay app and debug the splash screen.
#endif // HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH

	m_GameWorld.SetViewport( &GetClientViewport() );

	HE_LOG( Log, TEXT( "Engine startup complete." ) );
}

void HEngine::PostStartup()
{
	HE_LOG( Log, TEXT( "Beginning engine post-startup." ) );

	// TODO Get this from the DefaultEngine.ini
	String StartingWorldPath = FGameProject::GetInstance()->GetContentFolder() + "/Levels/TestLevel.hlevel";
	m_GameWorld.Initialize( StartingWorldPath.c_str() );

	String InputConfigPath = FGameProject::GetInstance()->GetConfigFolder() + "/DefaultInput.ini";
	GetClientViewport().GetInputDispatcher()->LoadMappingsFromFile( InputConfigPath.c_str() );

	GetClientViewport().Show();
	GetClientViewport().BringToFocus();
	if (IsPlayingInEditor())
	{
		m_GameWorld.BeginPlay();
		GGameInstance->OnGameSetFocus();
	}
	m_ReneringSubsystem.PushSceneForRendering( *m_GameWorld.GetScene() );

	m_IsInitialized = true;
	HE_LOG( Log, TEXT( "Engine post-startup complete." ) );
}

void HEngine::PreShutdown()
{
	HE_LOG( Log, TEXT( "Beginning engine pre-shutdown." ) );

	GCommandManager.IdleGpu();
	HE_LOG( Log, TEXT( "Engine pre-shutdown complete." ) );
}

void HEngine::Shutdown()
{
	HE_LOG( Log, TEXT( "Shutting down engine." ) );

	FApp::GetInstance()->Shutdown();
	GetClientViewport().Uninitialize();
	m_GameWorld.Flush();

	FAssetDatabase::Uninitialize();
	GMaterialManager.FlushMaterialCache();

	HE_LOG( Log, TEXT( "Engine shutdown complete." ) );
}

void HEngine::PostShutdown()
{
	HE_LOG( Log, TEXT( "Beginning engine post-shutdown." ) );
	
	FRendererInitializer::UnInitializeContext( m_RenderContext );
	HE_SAFE_DELETE_PTR( GThreadPool );

	System::UninitializePlatform();
	HE_SAFE_DELETE_PTR( GGameInstance );

	m_ReneringSubsystem.UnInitialize();
	m_PhysicsSubsystem.UnInitialize();

	HE_LOG( Log, TEXT( "Engine post-shutdown complete." ) );
}

void HEngine::Update()
{
	HE_LOG( Log, TEXT( "Entering Engine update loop." ) );

	m_FrameTimer.Initialize();
	while (FApp::GetInstance()->IsRunning())
	{
		System::ProcessMessages();
		TickTimers();
		float DeltaTime = (float)GetDeltaTime();

		GetClientViewport().Update( DeltaTime );

		// Check if the main viewport has focus. There will 
		// only be one window in shipping builds.
#if HE_STANDALONE
		if (!GetClientViewport().HasFocus())
		{
			BackgroundUpdate( DeltaTime );
			continue;
		}
#endif
		m_GameWorld.Tick( DeltaTime );

		RenderClientViewport( DeltaTime );
		
		static float SecondTimer = 0.f;
		static float FPS = 0.f;
		SecondTimer += DeltaTime;
		if (SecondTimer > 1.f)
		{
			HE_LOG( Log, TEXT( "FPS: %f" ), FPS );
			FPS = 0.f;
			SecondTimer = 0.f;
		}
		else
			FPS++;
	}

	HE_LOG( Log, TEXT( "Exiting Engine update loop." ) );
}

void HEngine::RenderClientViewport( float DeltaTime )
{
	GetClientViewport().Render();
}

void HEngine::BackgroundUpdate( float DeltaTime )
{

}

// Event Processing
//

void HEngine::OnEvent( Event& e )
{
	EventDispatcher Dispatcher( e );

	// Window
	Dispatcher.Dispatch<WindowClosedEvent>( this, &HEngine::OnClientWindowClosed );
}

bool HEngine::OnClientWindowClosed( WindowClosedEvent& e )
{
	RequestShutdown();
	return false;
}

void HEngine::RequestShutdown()
{
	HE_LOG( Log, TEXT( "Engine shutdown requested." ) );
	m_Application.RequestShutdown();
}


//
// Static function implementations
//

/* static */ void SplashMain( void* pUserData )
{
	HE_UNUSED_PARAM( pUserData );
	
	String SplashTextureDir =
#if HE_WITH_EDITOR
		FGameProject::GetInstance()->GetContentFolder() + "/Engine/Textures/Splash/HelixEd-Splash.dds";
#else
		"";
#endif
	FSplashScreen AppSplash( SplashTextureDir );

	while (!GEngine->IsInitialized())
	{
		FCommandContext& Context = FCommandContext::Begin( TEXT( "Scene Pass" ) );
		{
			AppSplash.Render( Context );
		}
		Context.End();
		AppSplash.EndFrame();
	}
}
