// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/HEngine.h"

#include "System.h"
#include "GameFramework/GameInstance.h"
#include "ThreadPool.h"
#include "FileSystem.h"
#include "System.h"

#include "IGpuResource.h"
#include "ICommandContext.h"
#include "ICommandManager.h"
#include "Engine/SplashScreen.h"
#include "Engine/Event/EngineEvent.h"


HEngine* GEngine = NULL;
Logger GEngineLogger;
ThreadPool* GThreadPool = NULL;


// Static function declarations
//
static void SplashMain(void* pUserData);

//
// Engine
//

HEngine::HEngine( CommandLine& CmdLine )
	: m_IsInitialized( false )
	, m_IsEditorPresent( CmdLine[L"-launchcfg"] == L"LaunchEditor" )
	, m_IsPlayingInEditor( false )
{
}

HEngine::~HEngine()
{
}

void HEngine::EngineMain()
{
	if ( IsInitialized() )
	{
		HE_LOG(Warning, TEXT("Trying to call Engine::EngineMain() on an engine instance that is already initialized!"));
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

	HE_LOG( Log, TEXT( "Engine pre-startup complete." ) );
}

void HEngine::Startup()
{
	HE_LOG(Log, TEXT("Starting up engine."));

	// Initialize the thread pool.
	GThreadPool = new ThreadPool(System::GetProcessorCount(), NULL);
	GThreadPool->SpawnWorkers();

	// Initialize the filesystem.
	FileSystem::Init();
#if HE_DEBUG_FILESYSTEM
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( sizeof(Path), Path );
	HE_LOG(Log, TEXT("FileSystem initialized with working directory: %s"), Path);
#endif

	// Initialize the renderer.
	ERenderBackend API = RB_Direct3D12; // TODO: Get from ini file
	RendererInitializer::InitializeContext( API, m_RenderContext );
	
	m_FrameTimeManager.Initialize();

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

	String AssetDatabaseRoot = FGameProject::GetInstance()->GetContentFolder() + "/DataCache/AssetDatabase.json";
	AssetDatabase::GetInstance()->Initialize( AssetDatabaseRoot.c_str() );

	// Create and initialize the main client window.
	Window::Description ClientDesc = {};
	ClientDesc.bHasTitleBar = true;
	ClientDesc.bShowImmediate = false;
	ClientDesc.Width = 1920;
	ClientDesc.Height = 1080;
	ClientDesc.Title = 
#if HE_WITH_EDITOR
	TEXT( "Helix Ed" ) " [" HE_PLATFORM_STRING " - " HE_CONFIG_STRING "]";
#else
	FApp::GetInstance()->GetName();
#endif
	ClientDesc.bAllowDropFiles = GetIsEditorPresent();
	m_MainViewPort.Initialize( ClientDesc );
	m_MainViewPort.GetWindow().AddListener( this, &HEngine::OnEvent );


#if HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH
	// Create the splash screen to serve as a loading indicator.
	GThreadPool->Kick(SplashMain, NULL);
	//std::this_thread::sleep_for(std::chrono::seconds(2)); // Uncomment to delay app and debug the splash screen.
#endif // HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH

	m_GameWorld.SetViewport( &GetClientViewport() );

	// TODO Get this from the DefaultEngine.ini
	String StartingWorldPath = FGameProject::GetInstance()->GetContentFolder() + "/Levels/TestLevel.hlevel";
	m_GameWorld.Initialize( StartingWorldPath.c_str() );
	GetClientViewport().SetWorld( &m_GameWorld );

	String InputConfigPath = FGameProject::GetInstance()->GetConfigFolder() + "/DefaultInput.ini";
	GetClientViewport().GetInputDispatcher()->LoadMappingsFromFile( InputConfigPath.c_str() );

	HE_LOG(Log, TEXT("Engine startup complete."));
}

void HEngine::PostStartup()
{
	HE_LOG(Log, TEXT("Beginning engine post-startup."));

	GetClientViewport().Show();
	GetClientViewport().BringToFocus();
#if HE_STANDALONE
	m_GameWorld.BeginPlay();
#endif

	//EWindowMode ClientWindowMode = WM_FullScreen;
	//m_MainViewPort.GetWindow().SetWindowMode(ClientWindowMode);

	m_IsInitialized = true;
	HE_LOG(Log, TEXT("Engine post-startup complete."));
}

void HEngine::PreShutdown()
{
	HE_LOG( Log, TEXT( "Beginning engine pre-shutdown." ) );

	HE_LOG( Log, TEXT( "Engine pre-shutdown complete." ) );
}

void HEngine::Shutdown()
{
	HE_LOG(Log, TEXT("Shutting down engine."));

	FApp::GetInstance()->Shutdown();
	GetClientViewport().Uninitialize();
	m_GameWorld.Flush();

	AssetDatabase::GetInstance()->UnInitialize();

	HE_LOG(Log, TEXT("Engine shutdown complete."));
}

void HEngine::PostShutdown()
{
	HE_LOG( Log, TEXT( "Beginning engine post-shutdown." ) );
	RendererInitializer::UnInitializeContext( m_RenderContext );
	HE_SAFE_DELETE_PTR(GThreadPool);
	System::UninitializePlatform();
	HE_LOG( Log, TEXT( "Engine post-shutdown complete." ) );
}

void HEngine::Update()
{
	HE_LOG( Log, TEXT( "Entering Engine update loop." ) );

	while ( FApp::GetInstance()->IsRunning() )
	{
		System::ProcessMessages();
		
		// The system could have processed a quit request. 
		// Check one more time before proceeding with the loop.
		if ( !FApp::GetInstance()->IsRunning() ) break;

		float DeltaTime = GetDeltaTime();
		
		// Check if the main viewport has focus. There will only be one window 
		// in shipping builds.
#if HE_STANDALONE
		if ( !GetClientViewport().HasFocus() )
		{
			BackgroundUpdate( DeltaTime );
			continue;
		}
#endif
		m_GameWorld.Tick( DeltaTime );
		
		RenderClientViewport( DeltaTime );

		m_FrameTimeManager.Tick( GetClientViewport().GetWindow().IsVSyncEnabled(), false );
	}
	HE_LOG( Log, TEXT( "Exiting Engine update loop." ) );
}

void HEngine::RenderClientViewport(float DeltaTime)
{
	ViewportContext& ClientViewport = GetClientViewport();
	ClientViewport.Update( DeltaTime );
	ClientViewport.Render();

	ICommandContext& CmdContext = ICommandContext::Begin( TEXT( "Present" ) );
	{
		IColorBuffer* pSwapChainSurface = GetClientViewport().GetWindow().GetRenderSurface();
		IGpuResource* SwapChainGpuResource = pSwapChainSurface->As<IGpuResource*>();

		CmdContext.TransitionResource( *SwapChainGpuResource, RS_Present );
	}
	CmdContext.End();

	ClientViewport.PresentOneFrame();
}

void HEngine::BackgroundUpdate(float DeltaTime)
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
	HE_LOG(Log, TEXT("Engine shutdown requested."));
	m_Application.RequestShutdown();
}


//
// Static function implementations
//

/* static */ void SplashMain( void* pUserData )
{
	(void)pUserData;
	String SplashTextureDir =
#if HE_WITH_EDITOR
		"../../Engine/Content/Textures/Splash/HelixEd-Splash.dds";
#else
		"";
#endif
	SplashScreen AppSplash( SplashTextureDir );

	while ( !GEngine->IsInitialized() )
	{
		ICommandContext& Context = ICommandContext::Begin(TEXT("Scene Pass"));
		{
			AppSplash.Render( Context );
		}
		Context.End();
		AppSplash.EndFrame();
	}
}
