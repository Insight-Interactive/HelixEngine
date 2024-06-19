#include "HelixEdPCH.h"

#include "Editor/EditorEngine.h"

#include "ThreadPool.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Tools/PackageMaker.h"
#include "GameFramework/Actor/ADebugPawn.h"
#include "Engine/Event/EngineEvent.h"
#include "Engine/FileExplorerWindow.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/Components/HFirstPersonCameraComponent.h"
#include "Texture.h"
#include "ColorBuffer.h"
#include "GpuResource.h"
#include "RenderDevice.h"
#include "CommandContext.h"
#include "TextureManager.h"
#include "CommandManager.h"
#include "Editor/Event/EditorEvent.h"


HEditorEngine* GEditorEngine = nullptr;
extern HGameInstance* GGameInstance;


static void RunActorEditor( void* pData );


HEditorEngine::HEditorEngine( FCommandLine& CmdLine )
	: Super( CmdLine )
	, m_HomeUI( m_MainViewPort )
	, m_UIContext( m_MainViewPort )
{
	if (GEditorEngine == nullptr)
		GEditorEngine = this;
	else
		HE_ASSERT( false ); // Only one editor engine can be present at any given time!
}

HEditorEngine::~HEditorEngine()
{
}

void HEditorEngine::PreStartup()
{
	Super::PreStartup();
}

void HEditorEngine::PostStartup()
{
	Super::PostStartup();
}

void HEditorEngine::Startup()
{
	Super::Startup();
	HE_LOG( Log, TEXT( "Starting up editor engine." ) );

	m_UIContext.Setup();

	char Path[HE_MAX_PATH];
	FGameProject::GetInstance()->GetConfigDirectoryFullPath( "imgui.ini", Path, sizeof( Path ) );
	ImGui::LoadIniSettingsFromDisk( Path );
	LoadEditorPreferences();

	m_HomeUI.SetupPanels();

	RegisterEditorOnlyAssets();
	HE_LOG( Log, TEXT( "Editor engine startup complete." ) );
}

void HEditorEngine::LoadEditorPreferences()
{
	rapidjson::Document PrefsJsonDoc;

	char Path[HE_MAX_PATH];
	FGameProject::GetInstance()->GetConfigDirectoryFullPath( "EditorPreferences.ini", Path, sizeof( Path ) );
	FileRef PrefsJsonSource( Path, FUM_Read );
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
	JsonUtility::WriteContext Writer( StrBuffer );

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
				FVector3 DebugCameraPos = m_HomeUI.GetDebugPawn()->GetRootComponent()->GetPosition();
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
				FVector3 DebugCameraRot = m_HomeUI.GetDebugPawn()->GetCameraComponent()->GetCameraAngles();
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
		//Writer.Double( m_HomeUI.GetDebugCharacter()->GetVerticalLookSpeed() );

		Writer.Key( HE_STRINGIFY( EditorPreferences::DebugCameraYawSpeed ) );
		//Writer.Double( m_HomeUI.GetDebugCharacter()->GetHorizontalLookSpeed() );
	}
	Writer.EndObject();

	char Path[HE_MAX_PATH];
	FGameProject::GetInstance()->GetConfigDirectoryFullPath( "EditorPreferences.ini", Path, sizeof( Path ) );
	FileRef OutFile( Path, FUM_Write, CM_Text );
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
	FGameProject::GetInstance()->GetConfigDirectoryFullPath( "imgui.ini", Path, sizeof( Path ) );
	ImGui::SaveIniSettingsToDisk( Path );
}

void HEditorEngine::RenderClientViewport( float DeltaTime )
{
	m_UIContext.BeginNewFrame();

	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2( 15, 6 );

	// Update the client world and viewport.
	m_MainViewPort.Render();

	// Wait for the current frame to finish rendering, we'll need it for the editor panels.

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
	m_HomeUI.Tick( DeltaTime );
	//m_PreferencesTab.Tick( DeltaTime );

	FColorBuffer& SwapChainSurface = m_MainViewPort.GetWindow().GetRenderSurface();
	FCommandContext& UIContext = FCommandContext::Begin( TEXT( "Draw Editor" ) );
	{
		UIContext.BeginDebugMarker( TEXT( "Draw Editor Panels" ) );
		{
			m_HomeUI.Render( UIContext );
			//m_PreferencesTab.Render( UIContext );
			// Render/Tick Actor Editors
			/*for (uint32 i = 0; i < m_ActorEditors.size(); i++)
			{
				m_ActorEditors[i]->Tick( DeltaTime );
				m_ActorEditors[i]->Render( UIContext );
			}*/
		}
		UIContext.EndDebugMarker();

		UIContext.TransitionResource( SwapChainSurface, RS_RenderTarget );
		UIContext.ClearColorBuffer( SwapChainSurface, m_MainViewPort.GetClientRect() );

		UIContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap() );
		const FColorBuffer* pRTs[] = { &SwapChainSurface };
		UIContext.OMSetRenderTargets( 1, pRTs, NULL );

		m_UIContext.EndFrame( UIContext );

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault( NULL, (void*)UIContext.GetNativeContext() );
		}

		UIContext.TransitionResource( SwapChainSurface, RS_Present );
	}
	UIContext.End();



	m_MainViewPort.PresentOneFrame();
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
		Style.Colors[ImGuiCol_WindowBg] = ImVec4( 0.15f, 0.15f, 0.15f, 1.f );
		Style.Colors[ImGuiCol_TitleBgActive] = ImVec4( 0.3f, 0.3f, 0.3f, 1.f );
		Style.Colors[ImGuiCol_TitleBg] = ImVec4( 0.3f, 0.3f, 0.3f, 1.f );

		Style.Colors[ImGuiCol_Tab] = ImVec4( 0.45f, 0.45f, 0.45f, 1.f );
		Style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.45f, 0.45f, 0.45f, 1.f );
		Style.Colors[ImGuiCol_TabActive] = ImVec4( 0.5f, 0.5f, 0.35f, 1.f );
		Style.Colors[ImGuiCol_Header] = ImVec4( 0.3f, 0.3f, 0.3f, 1.f );
	}
	else
	{
		ImGui::StyleColorsLight();
		Style.Colors[ImGuiCol_WindowBg] = ImVec4( .52f, .52f, .52f, 1.f );
	}
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
	Dispatcher.Dispatch<EngineBeginPlayEvent>( this, &HEditorEngine::OnAppBeginPlay );
	Dispatcher.Dispatch<EngineEndPlayEvent>( this, &HEditorEngine::OnAppEndPlay );
	Dispatcher.Dispatch<ObjectSelectedEvent>( this, &HEditorEngine::OnObjectSelected );

	// Editor
	Dispatcher.Dispatch<ContentItemDoubleClicked>( this, &HEditorEngine::OnContentItemClicked );

	if (m_MainViewPort.HasFocus())
		m_HomeUI.OnEvent( e );

	//if (m_PreferencesTab.HasFocus())
	//	m_PreferencesTab.OnEvent( e );

	for (uint32 i = 0; i < m_ActorEditors.size(); i++)
	{
		if (m_ActorEditors[i]->HasFocus())
			m_ActorEditors[i]->OnEvent( e );
	}
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
	FileExplorerWindow::Description Desc = {};
	Desc.FilterFoldersOnly = true;
	Desc.Title = L"Select a location to build the game to.";
	FileExplorerWindow Win;
	Win.Create( Desc );
	WChar* SelectedFolder;
	Win.Show( &SelectedFolder );

	HE_ASSERT( SelectedFolder != NULL );
	String GameTargetBuildDir = StringHelper::UTF16ToUTF8( SelectedFolder );


	const char* ProjLocation = FGameProject::GetInstance()->GetProjectRoot();
	const HName TGameName = FApp::GetInstance()->GetName();
	const String GameName = TCharToChar( TGameName );


	PackageMaker::BuildPackage( DebugGame, Win64, GameName.c_str(), ProjLocation, GameTargetBuildDir.c_str() );
}

void HEditorEngine::RegisterEditorOnlyAssets()
{
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
	return false;
}

bool HEditorEngine::OnClientWindowDropFile( WindowFileDropEvent& e )
{
	m_AssetImporter.Import( StringHelper::UTF16ToUTF8( e.GetFileName() ).c_str() );
	return true;
}

bool HEditorEngine::OnAppBeginPlay( EngineBeginPlayEvent& e )
{
	// Reset editor state if necessary.
	SetIsPlayingInEditor( true );

	// Start simulating the game.
	m_MainViewPort.GetInputDispatcher()->SetCanDispatchListeners( true );
	GGameInstance->OnGameSetFocus();
	// TODO Check if the world is dirty, ask to save if it first before playing.
	m_GameWorld.ReloadAndBeginPlay();
	
	return false;
}

bool HEditorEngine::OnAppEndPlay( EngineEndPlayEvent& e )
{
	// Reset editor state if necissary.
	SetIsPlayingInEditor( false );
	m_GameWorld.Reload();

	GGameInstance->OnGameLostFocus();
	// Reset the input state.
	m_MainViewPort.GetInputDispatcher()->SetCanDispatchListeners( false );
	m_MainViewPort.GetInputDispatcher()->FlushCallbacks();
	m_MainViewPort.GetWindow().MakeMoueWindowAssociation();

	return false;
}

bool HEditorEngine::OnObjectSelected( ObjectSelectedEvent& e )
{

	return false;
}

bool HEditorEngine::OnContentItemClicked( ContentItemDoubleClicked& e )
{
	String Title = StringHelper::GetFilenameFromDirectoryNoExtension( TCharToChar( e.GetClickedFilename() ) );
	ActorEditorTab* pNewEditor = new ActorEditorTab( CharToTChar( Title ), *this );
	pNewEditor->Activate();
	GThreadPool->Kick( RunActorEditor, pNewEditor );
	m_ActorEditors.push_back( pNewEditor );

	return false;
}

void HEditorEngine::OnExitMenuItem()
{
	RequestShutdown();
}

void HEditorEngine::OnSaveMenuItem()
{
	using namespace System;
	MessageDialogResult Result = CreateMessageBox( L"Are you sure you want to save the project?", L"Save Project?", MDI_OkCancel, MDIcon_Question );
	if (Result == MDR_Ok)
	{
		FAssetDatabase::SaveAssetDatabases();
		// World saves a reference to its own filepath, use null to satisfy the argument.
		m_GameWorld.Serialize( NULL );
	}
}

void HEditorEngine::OnEditorPreferencesMenuItem()
{
	//m_PreferencesTab.Activate();
}

void HEditorEngine::OnEditorPreferencesViewportClosed()
{
	//m_PreferencesTab.Deactivate();
}

void HEditorEngine::OnReloadPipelineShaders()
{
	HE_LOG( Log, TEXT( "Starting shader pipeline reload." ) );
	GCommandManager.IdleGpu();
	m_MainViewPort.ReloadRenderPipelines();
	HE_LOG( Log, TEXT( "Shader pipeline reload completed." ) );
}


// Static function implementations
//

void HEditorEngine::OnLaunchStandalone()
{
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( sizeof( Path ), Path );
	String ExePath = StringHelper::UTF16ToUTF8( Path );
	ExePath += "HelixEditor.exe";
	ExePath += " -launchcfg LaunchGame";

	system( ExePath.c_str() );
}

void RunActorEditor( void* pData )
{
	/*ImGuiContext* pImGuiCtx = ImGui::CreateContext();

	ActorEditorTab& Editor = *(ActorEditorTab*)pData;
	FTimer Timer;
	while (Editor.GetWindow().IsValid())
	{
		Timer.Tick();

		Editor.Tick( (float)Timer.GetTimeSeconds() );
		FCommandContext& CmdCtx = FCommandContext::Begin( TEXT( "Render Actor Editor" ) );
		{
			Editor.Render( CmdCtx );
		}
		CmdCtx.End();
	}*/
}