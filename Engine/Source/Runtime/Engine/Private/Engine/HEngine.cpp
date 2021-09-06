// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/HEngine.h"

#include "System.h"
#include "GameFramework/GameInstance.h"
#include "Jobs/ThreadPool.h"
#include "FileSystem.h"
#include "System.h"

#include "IGpuResource.h"
#include "ICommandContext.h"
#include "ICommandManager.h"
#include "Engine/SplashScreen.h"
#include "Engine/Event/EngineEvent.h"


HEngine* GEngine = NULL;
Logger GEngineLogger;


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
	RendererInitializer::UnInitializeContext( m_RenderContext );
}

void HEngine::EngineMain()
{
	if ( IsInitialized() )
	{
		HE_LOG(Warning, TEXT("Trying to call Engine::EngineMain() on an engine instance that is already initialized!"));
		HE_DEBUG_BREAK;
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
	HE_LOG( Log, TEXT( "Beginning engine pre-startup." ) );

	HE_LOG( Log, TEXT( "Engine pre-startup complete." ) );
}

void HEngine::Startup()
{
	CreateLogger(GEngineLogger, "Helix Engine");
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

	// Initialize the application context.
	FApp::GetInstance()->Startup();

	// Initialize the game.
	GGameInstance = MakeGameInstance();

	// Create and initialize the main client window.
	Window::Description ClientDesc = {};
	ClientDesc.bHasTitleBar = true;
	ClientDesc.bShowImmediate = false;
	ClientDesc.Width = 1920;
	ClientDesc.Height = 1080;
	ClientDesc.Title = m_Application.GetName();
#if HE_WITH_EDITOR
	ClientDesc.bAllowDropFiles = GetIsEditorPresent();
#endif //HE_WITH_EDITOR
	m_MainViewPort.Initialize( ClientDesc );
	m_MainViewPort.GetWindow().AddListener( this, &HEngine::OnEvent );

	AssetDatabase::GetInstance()->Initialize("TODO");

#if HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH
	// Create the splash screen to serve as a loading indicator.
	GThreadPool->Kick(SplashMain, NULL);
	//std::this_thread::sleep_for(std::chrono::seconds(4)); // Uncomment to delay app and debug the splash screen.

#endif // HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH

	HE_LOG(Log, TEXT("Engine startup complete."));
}

void HEngine::PostStartup()
{
	HE_LOG(Log, TEXT("Beginning engine post-startup."));

	GetClientViewport().Show();
	GetClientViewport().BringToFocus();
	//EWindowMode ClientWindowMode = WM_FullScreen;
	//m_MainViewPort.GetWindow().SetWindowMode(ClientWindowMode);

	m_IsInitialized = true;
	HE_LOG(Log, TEXT("Engine post-startup complete."));
}

void HEngine::PreShutdown()
{

}

void HEngine::Shutdown()
{
	HE_LOG(Log, TEXT("Shutting down engine."));

	m_Application.Shutdown();
	GetClientViewport().Uninitialize();

	AssetDatabase::GetInstance()->UnInitialize();

	HE_LOG(Log, TEXT("Engine shutdown complete."));
}

void HEngine::PostShutdown()
{
	HE_SAFE_DELETE_PTR(GThreadPool);
}

void HEngine::Update()
{
	HE_LOG( Log, TEXT( "Starting Engine update loop." ) );

	while ( FApp::GetInstance()->IsRunning() )
	{
		System::ProcessMessages();
		
		// The system could have processed a quit request. 
		// Check one more time before proceeding with the loop.
		if ( !FApp::GetInstance()->IsRunning() ) break;

		float DeltaTime = (float)m_FrameTimeManager.GetFrameTime();

		// Check if the main viewport has focus. There will only be one window 
		// in shipping builds.
#if HE_SHIPPING
		if ( !GetClientViewport().HasFocus() )
		{
			BackgroundUpdate();
			continue;
		}
#endif
		RenderClientViewport( DeltaTime );

		m_FrameTimeManager.Update( GetClientViewport().GetWindow().IsVSyncEnabled(), false );
	}
}

void HEngine::RenderClientViewport(float DeltaTime)
{
	GetClientViewport().Update( DeltaTime );
	GetClientViewport().Render();

	ICommandContext& CmdContext = ICommandContext::Begin( TEXT( "Present" ) );
	{
		IColorBuffer* pSwapChainSurface = GetClientViewport().GetWindow().GetRenderSurface();
		IGpuResource* SwapChainGpuResource = pSwapChainSurface->As<IGpuResource*>();

		CmdContext.TransitionResource( *SwapChainGpuResource, RS_Present );
	}
	CmdContext.End();
	GetClientViewport().PresentOneFrame();
}

void HEngine::BackgroundUpdate()
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
	String SplashTextureDir = "../../Engine/Content/Textures/Splash/HelixEd-Splash.dds";
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
