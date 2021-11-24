#include "HelixEdPCH.h"

#include "Editor/EditorEngine.h"

#include "System.h"
#include "StringHelper.h"
#include "ThreadPool.h"
#include "RenderDevice.h"
#include "GpuResource.h"
#include "TextureManager.h"
#include "CommandContext.h"
#include "CommandManager.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Engine/Event/EngineEvent.h"
#include "Developer/ADebugPawn.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "Tools/PackageMaker.h"
#include "Engine/FileExplorerWindow.h"
#include "GameFramework/GameInstance.h"

#include "Texture.h"
#include "ColorBuffer.h"
#include "CommandContext.h"
#include "RenderDevice.h"


HEditorEngine::HEditorEngine( CommandLine& CmdLine )
	: Super( CmdLine )
{
}

HEditorEngine::~HEditorEngine()
{
}

void HEditorEngine::PreStartup()
{
	Super::PreStartup();
}

void HEditorEngine::Startup()
{
	Super::Startup();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable FWindow Independence
	IO.ConfigWindowsMoveFromTitleBarOnly = true;
	IO.ConfigDockingAlwaysTabBar = true;
	IO.Fonts->AddFontFromFileTTF( "../../Engine/Content/Fonts/Cousine-Regular.ttf", 15.0f );
	IO.IniFilename = NULL;

	String ImGuiIniPath = FGameProject::GetInstance()->GetConfigFolder() + "/imgui.ini";
	ImGui::LoadIniSettingsFromDisk( ImGuiIniPath.c_str() );
	LoadEditorPreferences();

	EnableDarkMode( m_UserPreferences.EnableDarkMode );
	
	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowRounding = 0.0f;
	Style.Colors[ImGuiCol_WindowBg].w = 1.f;

	void* hwnd = *(HWND*)GetClientViewport().GetWindow().GetNativeWindow();
	bool ImGuiImplWin32Succeeded = ImGui_ImplWin32_Init( hwnd );
	HE_ASSERT( ImGuiImplWin32Succeeded );
	SetupImGuiRenderBackend();

	SetupEditorPanels();
}

void HEditorEngine::LoadEditorPreferences()
{
	rapidjson::Document PrefsJsonDoc;
	String EditorConfigPath = FGameProject::GetInstance()->GetConfigFolder() + "/DefaultEditor.ini";
	FileRef PrefsJsonSource( EditorConfigPath.c_str(), FUM_Read );
	HE_ASSERT( PrefsJsonSource->IsOpen() );
	JsonUtility::LoadDocument( PrefsJsonSource, PrefsJsonDoc );
	if (PrefsJsonDoc.IsObject())
	{
		JsonUtility::GetBoolean( PrefsJsonDoc, HE_STRINGIFY( EditorPreferences::EnableDarkMode ), m_UserPreferences.EnableDarkMode );
		JsonUtility::GetFloat( PrefsJsonDoc, HE_STRINGIFY( EditorPreferences::VerticalScrollSpeed ), m_UserPreferences.VerticalScrollSpeed );
		JsonUtility::GetFloat( PrefsJsonDoc, HE_STRINGIFY( EditorPreferences::HorizontalScrollSpeed ), m_UserPreferences.HorizontalScrollSpeed );
		auto PosIter = PrefsJsonDoc.FindMember( HE_STRINGIFY( EditorPreferences::DebugCameraPosition ) );
		auto& PosArrProperty = PosIter->value;
		if (PosArrProperty.IsArray())
		{
			const rapidjson::Value& Vector = PosArrProperty.GetArray()[0];
			JsonUtility::GetFloat( Vector, "X", m_UserPreferences.DebugCameraPosition.x );
			JsonUtility::GetFloat( Vector, "Y", m_UserPreferences.DebugCameraPosition.y );
			JsonUtility::GetFloat( Vector, "Z", m_UserPreferences.DebugCameraPosition.z );

		}
		auto RotIter = PrefsJsonDoc.FindMember( HE_STRINGIFY( EditorPreferences::DebugCameraRotation ) );
		auto& RotArrProperty = RotIter->value;
		if (RotArrProperty.IsArray())
		{
			const rapidjson::Value& Vector = RotArrProperty.GetArray()[0];
			JsonUtility::GetFloat( Vector, "X", m_UserPreferences.DebugCameraRotation.x );
			JsonUtility::GetFloat( Vector, "Y", m_UserPreferences.DebugCameraRotation.y );
			JsonUtility::GetFloat( Vector, "Z", m_UserPreferences.DebugCameraRotation.z );
		}

		JsonUtility::GetFloat( PrefsJsonDoc, HE_STRINGIFY( EditorPreferences::DebugCameraPitchSpeed ), m_UserPreferences.DebugCameraPitchSpeed );
		JsonUtility::GetFloat( PrefsJsonDoc, HE_STRINGIFY( EditorPreferences::DebugCameraYawSpeed ), m_UserPreferences.DebugCameraYawSpeed );
	}
}

void HEditorEngine::SaveEditorPreferences()
{
	rapidjson::StringBuffer StrBuffer;
	WriteContext Writer( StrBuffer );

	Writer.StartObject();
	{
		Writer.Key( HE_STRINGIFY( EditorPreferences::EnableDarkMode ) );
		Writer.Bool( m_UserPreferences.EnableDarkMode );

		Writer.Key( HE_STRINGIFY( EditorPreferences::VerticalScrollSpeed ) );
		Writer.Double( m_UserPreferences.VerticalScrollSpeed );

		Writer.Key( HE_STRINGIFY( EditorPreferences::HorizontalScrollSpeed ) );
		Writer.Double( m_UserPreferences.HorizontalScrollSpeed );

		Writer.Key( HE_STRINGIFY( EditorPreferences::DebugCameraPosition ) );
		Writer.StartArray();
		{
			Writer.StartObject();
			{
				FVector3 DebugCameraPos = m_SceneViewport.GetDebugPawn()->GetTransform().GetPosition();
				Writer.Key( "X" );
				Writer.Double( DebugCameraPos.x );
				Writer.Key( "Y" );
				Writer.Double( DebugCameraPos.y );
				Writer.Key( "Z" );
				Writer.Double( DebugCameraPos.z );
			}
			Writer.EndObject();
		}
		Writer.EndArray();

		Writer.Key( HE_STRINGIFY( EditorPreferences::DebugCameraRotation ) );
		Writer.StartArray();
		{
			Writer.StartObject();
			{
				FVector3 DebugCameraRot = m_SceneViewport.GetDebugPawn()->GetTransform().GetRotation();
				Writer.Key( "X" );
				Writer.Double( DebugCameraRot.x );
				Writer.Key( "Y" );
				Writer.Double( DebugCameraRot.y );
				Writer.Key( "Z" );
				Writer.Double( DebugCameraRot.z );
			}
			Writer.EndObject();
		}
		Writer.EndArray();

		Writer.Key( HE_STRINGIFY( EditorPreferences::DebugCameraPitchSpeed ) );
		Writer.Double( m_SceneViewport.GetDebugPawn()->GetVerticalLookSpeed() );

		Writer.Key( HE_STRINGIFY( EditorPreferences::DebugCameraYawSpeed ) );
		Writer.Double( m_SceneViewport.GetDebugPawn()->GetHorizontalLookSpeed() );
	}
	Writer.EndObject();

	String EditorConfigPath = FGameProject::GetInstance()->GetConfigFolder() + "/DefaultEditor.ini";
	FileRef OutFile( EditorConfigPath, FUM_Write, CM_Text );
	HE_ASSERT( OutFile->IsOpen() );
	if (OutFile->IsOpen())
	{
		if (!OutFile->WriteData( (void*)StrBuffer.GetString(), StrBuffer.GetSize(), 1 ))
		{
			HE_LOG( Error, TEXT( "Failed to save editor user preferences!" ) );
			HE_ASSERT( false );
		}
	}

	// Save the editor layout.
	ImGui::SaveIniSettingsToDisk((FGameProject::GetInstance()->GetConfigFolder() + "/imgui.ini").c_str());
}

void HEditorEngine::SetupImGuiRenderBackend()
{
	FSwapChain* pClientSwapChain = GetClientViewport().GetWindow().GetSwapChain();
	DXGI_FORMAT SwapChainFmt = (DXGI_FORMAT)pClientSwapChain->GetBackBufferFormat();
	int NumBackBuffers = pClientSwapChain->GetNumBackBuffers();
	
#if HE_WINDOWS
	ID3D12Device* pD3D12Device = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
	bool ImGuiD3D12Succeeded = ImGui_ImplDX12_Init( pD3D12Device, NumBackBuffers, SwapChainFmt );
	HE_ASSERT( ImGuiD3D12Succeeded );

#endif
}

void StandaloneGameLaunch( void* )
{
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( sizeof( Path ), Path );
	String ExePath = StringHelper::UTF16ToUTF8( Path );
	ExePath += "Engine.exe";
	ExePath += " -launchcfg LaunchGame";

	system( ExePath.c_str() );
}

void HEditorEngine::RenderClientViewport( float DeltaTime )
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport( NULL, ImGuiDockNodeFlags_PassthruCentralNode );

	// Update the client world and viewport.
	GetClientViewport().Update( DeltaTime );
	GetClientViewport().Render();

	/*if (m_PreferencesViewport.IsValid())
	{
		m_PreferencesViewport.Update( DeltaTime );
		m_PreferencesViewport.Render();
	}*/

	// ----------------------
	// For later reference
	// ----------------------
	// 
	//ImGui::Begin( "Hello From HelixEd!" );
	//{
	//	static HTextureRef pTexture;
	//	static bool Loaded = false;
	//	if (!Loaded)
	//	{
	//		Loaded = true;
	//		pTexture = GTextureManager.LoadTexture( "Content/Textures/RustedIron/RustedIron_Albedo.dds", DT_Magenta2D, false );
	//	}
	//	const HTextureD3D12* pD3D12Tex = DCast<const HTextureD3D12*>( pTexture.Get() );
	//	D3D12_GPU_DESCRIPTOR_HANDLE TextureHandle{ pD3D12Tex->GetShaderVisibleDescriptorHandle().GetGpuPtr() };
	//	ImGui::Text( "Texture: %s", pTexture.GetCacheKey().c_str() );
	//	ImGui::Image( (ImTextureID)TextureHandle.ptr, ImVec2( 200, 200 ) );
	//	/*static bool Pressed = false;
	//	if (ImGui::Button( "Start New Instance" ) && !Pressed)
	//	{
	//		Pressed = true;
	//		GThreadPool->Kick( StandaloneGameLaunch, NULL );
	//	}*/
	//}
	//ImGui::End();

	ImGui::Begin( "Set Window Mode" );
	{
		if (ImGui::Button( "Windowed" ))
		{
			GetClientViewport().SetWindowMode( WM_Windowed );
		}
		if (ImGui::Button( "Borderless" ))
		{
			GetClientViewport().SetWindowMode( WM_Borderless );
		}
		if (ImGui::Button( "Fullscreen" ))
		{
			GetClientViewport().SetWindowMode( WM_FullScreen );
		}
	}
	ImGui::End();


	FColorBuffer* pSwapChainSurface = GetClientViewport().GetWindow().GetRenderSurface();
	FCommandContext& UIContext = FCommandContext::Begin( TEXT( "Draw Editor" ) );
	{
		UIContext.BeginDebugMarker( TEXT( "Draw Panels" ) );
		{
			for (size_t i = 0; i < m_EditorPanels.size(); ++i)
			{
				m_EditorPanels[i]->Tick( DeltaTime );
				m_EditorPanels[i]->Render( UIContext );
			}
		}
		UIContext.EndDebugMarker();

		ImGui::Render();

		FGpuResource& SwapChainGpuResource = *DCast<FGpuResource*>( pSwapChainSurface );
		UIContext.TransitionResource( SwapChainGpuResource, RS_RenderTarget );
		UIContext.ClearColorBuffer( *pSwapChainSurface, GetClientViewport().GetClientRect() );

		UIContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap() );
		const FColorBuffer* pRTs[] = {
			pSwapChainSurface
		};
		UIContext.OMSetRenderTargets( 1, pRTs, NULL );

		ID3D12CommandList* pCommandList = RCast<ID3D12CommandList*>( UIContext.GetNativeContext() );

		ImGui_ImplDX12_RenderDrawData( ImGui::GetDrawData(), (ID3D12GraphicsCommandList*)pCommandList );

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault( NULL, (void*)pCommandList );
		}
		UIContext.TransitionResource( SwapChainGpuResource, RS_Present );
	}
	UIContext.End();

	GetClientViewport().PresentOneFrame();
}

void HEditorEngine::Shutdown()
{
	Super::Shutdown();

	SaveEditorPreferences();

	ImGui::DestroyContext();
}

void HEditorEngine::EnableDarkMode( bool Enabled )
{
	ImGuiStyle& Style = ImGui::GetStyle();
	if (Enabled)
	{
		ImGui::StyleColorsDark();
		Style.Colors[ImGuiCol_WindowBg] = ImVec4( .15f, .15f, .15f, 1.f );
		Style.Colors[ImGuiCol_TitleBgActive] = ImVec4( .3f, .3f, .3f, 1.f );
		Style.Colors[ImGuiCol_TitleBg] = ImVec4( .3f, .3f, .3f, 1.f );

		Style.Colors[ImGuiCol_Tab] = ImVec4( .45f, .45f, .45f, 1.f );
		Style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4( .45f, .45f, .45f, 1.f );
		Style.Colors[ImGuiCol_TabActive] = ImVec4( .5f, .5f, .35f, 1.f );
		Style.Colors[ImGuiCol_Header] = ImVec4( .3f, .3f, .3f, 1.f );
	}
	else
	{
		ImGui::StyleColorsLight();
		Style.Colors[ImGuiCol_WindowBg] = ImVec4( .52f, .52f, .52f, 1.f );
	}
}

void HEditorEngine::SetupEditorPanels()
{
	m_EditorPanels.push_back( &m_MenuBar );
	m_EditorPanels.push_back( &m_ToolbarPanel );
	m_EditorPanels.push_back( &m_SceneViewport );
	m_EditorPanels.push_back( &m_ConsoleOutputPanel );
	m_EditorPanels.push_back( &m_ContentBrowserPanel );
	m_EditorPanels.push_back( &m_WorldOutline );
	m_EditorPanels.push_back( &m_DetailsPanel );

	for (size_t i = 0; i < m_EditorPanels.size(); ++i)
	{
		m_EditorPanels[i]->SetOwningViewport( &GetClientViewport() );
		m_EditorPanels[i]->Initialize();
	}

	m_MenuBar.AddMenuItem( "File", "Exit", this, &HEditorEngine::OnExitMenuItem );
	m_MenuBar.AddMenuItem( "File", "Save", this, &HEditorEngine::OnSaveMenuItem );
	m_MenuBar.AddMenuItem( "File", "Package Game", this, &HEditorEngine::PackageGame );
	//m_MenuBar.AddMenuItem( "Edit", "Preferences", this, &HEditorEngine::OnEditorPreferencesMenuItem );
	m_MenuBar.AddMenuItem( "Developer", "Reload Pipeline Shaders", this, &HEditorEngine::OnReloadPipelineShaders );

	m_ToolbarPanel.AddListener( this, &HEditorEngine::OnEvent );
	ADebugPawn* pDebugPawn = m_SceneViewport.GetDebugPawn();
	pDebugPawn->GetTransform().SetPosition( m_UserPreferences.DebugCameraPosition );
	pDebugPawn->GetTransform().SetRotation( m_UserPreferences.DebugCameraRotation );
	pDebugPawn->SetVerticalLookSpeed( m_UserPreferences.DebugCameraPitchSpeed );
	pDebugPawn->SetHorizontalLookSpeed( m_UserPreferences.DebugCameraYawSpeed );
	m_WorldOutline.AddListener( this, &HEditorEngine::OnEvent );
}

//
// Event Processing
//


void HEditorEngine::OnEvent( Event& e )
{
	EventDispatcher Dispatcher( e );

	// Key Events
	Dispatcher.Dispatch<KeyPressedEvent>( this, &HEditorEngine::OnKeyPressed );
	Dispatcher.Dispatch<KeyReleasedEvent>( this, &HEditorEngine::OnKeyReleased );

	// Mouse Events
	Dispatcher.Dispatch<MousePositionMovedEvent>( this, &HEditorEngine::OnMousePositionMoved );
	Dispatcher.Dispatch<MouseButtonPressedEvent>( this, &HEditorEngine::OnMouseButtonPressed );
	Dispatcher.Dispatch<MouseButtonReleasedEvent>( this, &HEditorEngine::OnMouseButtonReleased );
	Dispatcher.Dispatch<MouseWheelScrolledEvent>( this, &HEditorEngine::OnMouseWheelScrolled );

	// FWindow
	Dispatcher.Dispatch<WindowResizeEvent>( this, &HEditorEngine::OnWindowResized );
	Dispatcher.Dispatch<WindowFocusEvent>( this, &HEditorEngine::OnWindowFocus );
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &HEditorEngine::OnWindowLostFocus );
	Dispatcher.Dispatch<WindowClosedEvent>( this, &HEditorEngine::OnClientWindowClosed );
	Dispatcher.Dispatch<WindowFileDropEvent>( this, &HEditorEngine::OnClientWindowDropFile );

	// Application/Engine
	Dispatcher.Dispatch<AppBeginPlayEvent>( this, &HEditorEngine::OnAppBeginPlay );
	Dispatcher.Dispatch<AppEndPlayEvent>( this, &HEditorEngine::OnAppEndPlay );
	Dispatcher.Dispatch<ObjectSelectedEvent>( this, &HEditorEngine::OnObjectSelected );
}

bool HEditorEngine::OnKeyPressed( KeyPressedEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[e.GetKeyCode()] = true;

	return false;
}

bool HEditorEngine::OnKeyReleased( KeyReleasedEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[e.GetKeyCode()] = false;

	return false;
}

bool HEditorEngine::OnMousePositionMoved( MousePositionMovedEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2( (float)e.GetX(), (float)e.GetY() );

	return false;
}

bool HEditorEngine::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[TranslateMouseButton( e.GetKeyCode() )] = true;

	return false;
}

bool HEditorEngine::OnMouseButtonReleased( MouseButtonReleasedEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[TranslateMouseButton( e.GetKeyCode() )] = false;

	return false;
}

bool HEditorEngine::OnMouseWheelScrolled( MouseWheelScrolledEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	float XOffset = e.GetXOffset() * m_UserPreferences.VerticalScrollSpeed;
	float YOffset = e.GetYOffset() * m_UserPreferences.HorizontalScrollSpeed;
	if (XOffset > 0.f)
		io.MouseWheelH = XOffset;
	else
		io.MouseWheel = YOffset;

	return false;
}

bool HEditorEngine::OnWindowResized( WindowResizeEvent& e )
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2( (float)e.GetWidth(), (float)e.GetHeight() );
	io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );

	return false;
}

int32 HEditorEngine::TranslateMouseButton( DigitalInput MouseKeycode )
{
	HE_ASSERT( IsValidMouseButton( MouseKeycode ) );

	return MouseKeycode - Mouse0;
}

void HEditorEngine::PackageGame()
{
	FileExplorerWindow::Description Desc = { 0 };
	Desc.FilterFoldersOnly = true;
	Desc.Title = L"Select a location to build the game to.";
	FileExplorerWindow Win;
	Win.Create( Desc );
	WChar* SelectedFolder;
	Win.Show( &SelectedFolder );

	HE_ASSERT( SelectedFolder != NULL );
	String GameTargetBuildDir = StringHelper::UTF16ToUTF8( SelectedFolder );

	
	const String& ProjLocation = FGameProject::GetInstance()->GetProjectRoot();
	const HName TGameName = FApp::GetInstance()->GetName();
	const String GameName = TCharToChar( TGameName );


	PackageMaker::BuildPackage( DebugGame, Win64, GameName.c_str(), ProjLocation.c_str(), GameTargetBuildDir.c_str() );
}

bool HEditorEngine::OnWindowLostFocus( WindowLostFocusEvent& e )
{

	return false;
}

bool HEditorEngine::OnWindowFocus( WindowFocusEvent& e )
{

	return false;
}

bool HEditorEngine::OnClientWindowClosed( WindowClosedEvent& e )
{
	RequestShutdown();
	if (FAssetDatabase::GetInstance()->IsAnyDatabaseDirty())
	{
		OnSaveMenuItem();
	}
	return false;
}

bool HEditorEngine::OnClientWindowDropFile( WindowFileDropEvent& e )
{
	m_AssetImporter.Import( StringHelper::UTF16ToUTF8( e.GetFileName() ).c_str() );
	return true;
}

bool HEditorEngine::OnAppBeginPlay( AppBeginPlayEvent& e )
{
	SetIsPlayingInEditor( true );
	m_SceneViewport.DeactivateDebugCamera();

	GetClientViewport().GetInputDispatcher()->SetCanDispatchListeners( true );
	GGameInstance->OnGameSetFocus();

	// TODO Check if the world is dirty, ask to save if it first before playing.
	m_GameWorld.ReloadAndBeginPlay();

	return false;
}

bool HEditorEngine::OnAppEndPlay( AppEndPlayEvent& e )
{
	SetIsPlayingInEditor( false );
	m_GameWorld.Reload();
	m_SceneViewport.ActivateDebugCamera();

	// Reset the input state.
	GetClientViewport().GetInputDispatcher()->SetCanDispatchListeners( false );
	GetClientViewport().GetInputDispatcher()->FlushCallbacks();
	GetClientViewport().GetWindow().MakeMoueWindowAssociation();

	return false;
}

bool HEditorEngine::OnObjectSelected( ObjectSelectedEvent& e )
{
	m_DetailsPanel.SetSelectedObject( e.GetSelectedObject() );

	return false;
}

void HEditorEngine::OnExitMenuItem()
{
	RequestShutdown();
}

void HEditorEngine::OnSaveMenuItem()
{
	using namespace System;
	MessageDialogResult Result = CreateMessageBox( L"Are you sure you want to save the project?", L"Save Project?", MDI_OkCancel, System::MessageDialogIcon::MDIcon_Question);
	if (Result == MDR_Ok)
	{
		FAssetDatabase::GetInstance()->SaveAssetDatabases();
		// World saves a reference to its own filepath, use null to satisfy the argument.
		m_GameWorld.Serialize( NULL );
	}
}

void HEditorEngine::OnEditorPreferencesMenuItem()
{
	if (m_PreferencesViewport.IsValid())
	{
		// If already open, bring it to focus.
		m_PreferencesViewport.BringToFocus();
		return;
	}

	FWindow::Description Desc = { };
	Desc.bHasTitleBar = true;
	Desc.bShowImmediate = true;
	Desc.Width = 1920;
	Desc.Height = 1080;
	Desc.Title = TEXT( "Editor Preferences" );
	m_PreferencesViewport.Initialize( Desc );
}

void HEditorEngine::OnEditorPreferencesViewportClosed()
{
	m_PreferencesViewport.Uninitialize();
}

void HEditorEngine::OnReloadPipelineShaders()
{
	HE_LOG( Log, TEXT( "Starting shader pipeline reload." ) );
	GCommandManager.IdleGpu();
	GetClientViewport().ReloadRenderPipelines();
	HE_LOG( Log, TEXT( "Shader pipeline reload completed." ) );
}
