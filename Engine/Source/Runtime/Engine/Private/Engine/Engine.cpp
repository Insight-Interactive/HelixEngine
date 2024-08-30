// Copyright 2024 Insight Interactive. All Rights Reserved.
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
#include "Engine/SplashScreen.h"
#include "Engine/Event/EngineEvent.h"
//#include "../Script/ScriptCallbacks.h"
#include "Renderer/FontManager.h"


Logger GEngineLogger;
HEngine* GEngine = nullptr;
ThreadPool* GThreadPool = nullptr;
HGameInstance* GGameInstance = nullptr;

extern HGameInstance* MakeGameInstance();


// Static function declarations
//
static void SplashMain( void* pUserData );

//
// Engine
//

HEngine::HEngine( FCommandLine& CmdLine )
	: m_IsInitialized( false )
	, m_IsEditorPresent( CmdLine.ArgumentEquals( L"-launchcfg", L"LaunchEditor" ) )
	, m_IsPlayingInEditor( !m_IsEditorPresent )
	, m_AppStartTime( 0 )
	, m_FrameTimeScale( 1.f )
	, m_FrameTime ( 0.f )
	, m_FrameStartTick ( 0 )
{
}

HEngine::~HEngine()
{
}

void HEngine::EngineMain()
{
	HE_ASSERTEX( !IsInitialized(), TEXT("Trying to call Engine::EngineMain() on an engine instance that is already initialized!") )

	// Startup.
	PreStartup();
	Startup();
	PostStartup();

	// Update the engine.
	Tick();

	// Shutdown.
	PreShutdown();
	Shutdown();
	PostShutdown();
}

void HEngine::PreStartup()
{
	CreateLogger( GEngineLogger, "Helix Engine" );
	HE_LOG( Log, TEXT( "Beginning engine pre-startup." ) );

	EmitEvent( EnginePreStartupEvent() );

	// Initialize all core systems.

	System::InitializePlatform();
	SystemTime::Initialize();
	m_AppStartTime = SystemTime::GetCurrentTick();

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

	// Initialize Subsystems
	Input::Initialize();
	Physics::Initialize();
	m_ReneringSubsystem.Initialize();
	m_ScriptSubsystem.Setup();
	m_ScriptSubsystem.BindLuaFunction( "GetDeltaTime", *this, &HEngine::GetDeltaTime );
	m_ScriptSubsystem.BindLuaFunction( "GetAppSeconds", *this, &HEngine::GetAppSeconds );

	m_ReneringSubsystem.RunAsync();

	HE_LOG( Log, TEXT( "Engine pre-startup complete. (Took %f seconds)" ), SystemTime::TimeBetweenTicks(m_AppStartTime, SystemTime::GetCurrentTick() ) );
}

void HEngine::Startup()
{
	HE_LOG( Log, TEXT( "Starting up engine." ) );

	EmitEvent( EngineStartupEvent() );

	// Startup the game and load all client facing resources.
	uint64 StartupTick = SystemTime::GetCurrentTick();

	// TODO: Make this dynamic
	FPath HProjectPath;
#if HE_STANDALONE && !HE_DEMO_GAME
		HProjectPath.SetPath( "Data\\Game.hproject" );
#else
		HProjectPath.SetPath( "C:\\Dev\\InsightInteractive\\HelixEngine\\Game\\Game.hproject" );
#endif
	FGameProject::GetInstance()->Startup( HProjectPath );

	// Initialize the application context.
	FApp::GetInstance()->Startup();

	// Initialize the game.
	GGameInstance = MakeGameInstance();
	GGameInstance->SetWorld( &GetGameWorld() );
	AddListener( GGameInstance, &HGameInstance::OnEvent );
	//GGameInstance = new HGameInstance();

	FAssetDatabase::Initialize();

	// Create and initialize the main client window.
	FWindow::Description ClientDesc = {};
	ClientDesc.bHasTitleBar		= true;
	ClientDesc.bShowImmediate	= false;
	ClientDesc.Resolution		= GCommonResolutions[k1080p]; 
#if HE_WITH_EDITOR
	HName EngineTitle = TEXT( "Helix Editor" );
	EngineTitle += TEXT( " (" );
	EngineTitle += CharToTChar( FGameProject::GetInstance()->GetProjectName() );
	EngineTitle += TEXT( ")" );
	EngineTitle += TEXT( " [" HE_PLATFORM_STRING " - " HE_CONFIG_STRING "]" );
	if (!GetIsEditorPresent())
		EngineTitle += TEXT( " (Standalone)" );

	ClientDesc.Title = EngineTitle.c_str();
#else
	ClientDesc.Title = FApp::GetInstance()->GetName();
#endif
	ClientDesc.bAllowDropFiles = GetIsEditorPresent();
	m_MainViewPort.Initialize( ClientDesc );
	m_MainViewPort.GetWindow().AddListener( this, &HEngine::OnEvent );

#if HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH
	// Create the splash screen to serve as a loading indicator to the user.
	//GThreadPool->Kick( SplashMain, NULL );
#endif // HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH

	m_GameWorld.SetViewport( &m_MainViewPort );

	HE_LOG( Log, TEXT( "Engine startup complete. (Took %f seconds)" ), SystemTime::TimeBetweenTicks( StartupTick, SystemTime::GetCurrentTick() ) );
}

void HEngine::PostStartup()
{
	HE_LOG( Log, TEXT( "Beginning engine post-startup." ) );

	EmitEvent( EnginePostStartupEvent() );

	FPath DefaultLevelPath;
	FGameProject::GetInstance()->GetDefaultLevelPath( DefaultLevelPath );
	m_GameWorld.Initialize( DefaultLevelPath.GetFullPath() );

	char Path[HE_MAX_PATH];
	FGameProject::GetInstance()->GetConfigDirectoryFullPath( "InputMappings.ini", Path, sizeof( Path ) );
	m_MainViewPort.GetInputDispatcher()->LoadMappingsFromFile( Path );

	m_MainViewPort.Show();
	m_MainViewPort.BringToFocus();
	if (IsPlayingInEditor())
	{
		EmitEvent( EngineBeginPlayEvent() );
		m_GameWorld.BeginPlay();
		GGameInstance->BeginPlay();
	}

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
	m_MainViewPort.Uninitialize();
	m_GameWorld.Flush();

	FAssetDatabase::Uninitialize();
	GMaterialManager.FlushMaterialCache();

	Input::UnInitialize();
	Physics::UnInitialize();

	HE_LOG( Log, TEXT( "Engine shutdown complete." ) );
}

void HEngine::PostShutdown()
{
	HE_LOG( Log, TEXT( "Beginning engine post-shutdown." ) );

	HE_SAFE_DELETE_PTR( GThreadPool );

	System::UninitializePlatform();
	HE_SAFE_DELETE_PTR( GGameInstance );

	m_ReneringSubsystem.UnInitialize();

	HE_LOG( Log, TEXT( "Engine post-shutdown complete." ) );
}

void HEngine::Tick()
{
	HE_LOG( Log, TEXT( "Entering Engine update loop." ) );
	Input::KbmZeroInputs();
	
	constexpr float SimulationStepRate = 1.f / 60.f;
	float StepAccumulator = 0.f;
	
	// Main loop.
	while ( m_Application.IsRunning() )
	{
		System::ProcessMessages();
		
		float DeltaTime = GetDeltaTime();
		EmitEvent( EngineTickEvent( DeltaTime ) );

		Input::Update( DeltaTime );
		Physics::Update( m_FrameTime, m_FrameTimeScale );

		m_MainViewPort.Tick( DeltaTime );
		GGameInstance->Tick(DeltaTime );

		StepAccumulator += DeltaTime;
		if (StepAccumulator > SimulationStepRate)
			m_GameWorld.FixedUpdate( SimulationStepRate );
		StepAccumulator -= SimulationStepRate;

		m_GameWorld.Tick( DeltaTime );


		EmitEvent( EngineRenderEvent() );
		RenderClientViewport( DeltaTime );

		TickTimers();
	}

	HE_LOG( Log, TEXT( "Exiting Engine update loop." ) );
}

void HEngine::RenderClientViewport( float DeltaTime )
{
	m_MainViewPort.Render();
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
	Dispatcher.Dispatch<WindowFocusEvent>( this, &HEngine::OnWindowFocus );
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &HEngine::OnWindowLostFocus );
}

bool HEngine::OnClientWindowClosed( WindowClosedEvent& e )
{
	RequestShutdown();
	return false;
}

bool HEngine::OnWindowFocus( WindowFocusEvent& e )
{
	GGameInstance->OnGameSetFocus();

	return false;
}

bool HEngine::OnWindowLostFocus( WindowLostFocusEvent& e )
{
	GGameInstance->OnGameLostFocus();

	return false;
}

void HEngine::RequestShutdown()
{
	HE_LOG( Log, TEXT( "Engine shutdown requested." ) );
	m_Application.Terminate();
}


//
// Static function implementations
//

/* static */ void SplashMain( void* pUserData )
{
	HE_UNUSED_PARAM( pUserData );

	char TexturePath[HE_MAX_PATH];
#if HE_WITH_EDITOR || HE_DEMO_GAME
		FGameProject::GetInstance()->GetContentDirectoryFullPath( "/Engine/Textures/Splash/HelixEd-Splash.dds", TexturePath, sizeof(TexturePath) );
#else
		ZeroMemory( TexturePath, sizeof( TexturePath ) );
		""; // TODO: Custom game splash image.
#endif
	FSplashScreen AppSplash( TexturePath );

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
