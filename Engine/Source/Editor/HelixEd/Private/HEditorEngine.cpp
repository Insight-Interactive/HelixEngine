#include "HelixEdPCH.h"

#include "Editor/HEditorEngine.h"

#include "System.h"
#include "StringHelper.h"
#include "Jobs/ThreadPool.h"
#include "IDevice.h"
#include "IGpuResource.h"
#include "ITextureManager.h"
#include "ICommandContext.h"
#include "ICommandManager.h"
#include "Direct3D12/TextureD3D12.h"
#include "Direct3D12/ColorBufferD3D12.h"
#include "Direct3D12/CommandContextD3D12.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Engine/Event/EngineEvent.h"


HEditorEngine::HEditorEngine( CommandLine& CmdLine )
	: HEngine( CmdLine )
{
}

HEditorEngine::~HEditorEngine()
{
}

void HEditorEngine::PreStartup()
{
	HEngine::PreStartup();
}

void HEditorEngine::Startup()
{
	HEngine::Startup();


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Window Independence
	IO.ConfigWindowsMoveFromTitleBarOnly = true;
	IO.ConfigDockingAlwaysTabBar = true;
	IO.Fonts->AddFontFromFileTTF( "../../Engine/Content/Fonts/Cousine-Regular.ttf", 15.0f );

	// TODO: Get this from a editor preferences ini
	m_UserPreferences.EnableDarkMode = true;
	m_UserPreferences.VerticalScrollSpeedMultiplier = 0.1f;
	m_UserPreferences.HorizontalScrollSpeedMultiplier = 0.1f;

	EnableDarkMode( m_UserPreferences.EnableDarkMode );

	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowRounding = 0.0f;
	Style.Colors[ImGuiCol_WindowBg].w = 1.f;

	void* hwnd = GetClientViewport().GetWindow().GetNativeWindow();
	bool ImGuiImplWin32Succeeded = ImGui_ImplWin32_Init( hwnd );
	HE_ASSERT( ImGuiImplWin32Succeeded );
	SetupImGuiRenderBackend();

	SetupEditorPanels();
}

void HEditorEngine::SetupImGuiRenderBackend()
{
	ISwapChain* pClientSwapChain = GetClientViewport().GetWindow().GetSwapChain();
	DXGI_FORMAT SwapChainFmt = (DXGI_FORMAT)pClientSwapChain->GetBackBufferFormat();
	int NumBackBuffers = pClientSwapChain->GetNumBackBuffers();

	switch (RenderContext::GetInstance()->GetBackendType())
	{
	case RB_Direct3D11:
		HE_ASSERT( false );
		break;
	case RB_Direct3D12:
	{
		ID3D12Device* pD3D12Device = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
		bool ImGuiD3D12Succeeded = ImGui_ImplDX12_Init( pD3D12Device, NumBackBuffers, SwapChainFmt );
		HE_ASSERT( ImGuiD3D12Succeeded );
		break;
	}
	default:
		HE_ASSERT( false );
	}
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
	ImGui::DockSpaceOverViewport( 0, ImGuiDockNodeFlags_PassthruCentralNode );

	// Update the client world and viewport.
	GetClientViewport().Update( DeltaTime );
	GetClientViewport().Render();


	ImGui::Begin( "Hello From HelixEd!" );
	{
		static TextureRef pTexture;
		static bool Loaded = false;
		if (!Loaded)
		{
			Loaded = true;
			pTexture = GTextureManager->LoadTexture( "Content/Textures/RustedIron/RustedIron_Albedo.dds", DT_Magenta2D, false );
		}
		const TextureD3D12* pD3D12Tex = DCast<const TextureD3D12*>( pTexture.Get() );
		D3D12_GPU_DESCRIPTOR_HANDLE TextureHandle{ pD3D12Tex->GetShaderVisibleDescriptorHandle().GetGpuPtr() };
		ImGui::Text( "Texture: %s", pTexture.GetCacheKey().c_str() );
		ImGui::Image( (ImTextureID)TextureHandle.ptr, ImVec2( 200, 200 ) );

		/*static bool Pressed = false;
		if (ImGui::Button( "Start New Instance" ) && !Pressed)
		{
			Pressed = true;
			GThreadPool->Kick( StandaloneGameLaunch, NULL );
		}*/
	}
	ImGui::End();


	ICommandContext& Context = ICommandContext::Begin( TEXT( "Setup Editor Display" ) );
	{
		for (size_t i = 0; i < m_EditorPanels.size(); ++i)
		{
			m_EditorPanels[i]->Tick( DeltaTime );
			m_EditorPanels[i]->Render( Context );
		}
	}
	Context.End();

	ImGui::Render();


	IColorBuffer* pSwapChainSurface = GetClientViewport().GetWindow().GetRenderSurface();

	ICommandContext& UIContext = ICommandContext::Begin( TEXT( "Draw Editor" ) );
	{
		// TODO: Doesnt't work for some reason. Only works in ViewportContext.cpp
		IGpuResource& SwapChainGpuResource = *DCast<IGpuResource*>( pSwapChainSurface );
		UIContext.TransitionResource( SwapChainGpuResource, RS_RenderTarget );
		UIContext.ClearColorBuffer( *pSwapChainSurface, GetClientViewport().GetClientRect() );

		UIContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap );
		const IColorBuffer* pRTs[] = {
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
	}
	UIContext.End();


	ICommandContext& PresentContext = ICommandContext::Begin( TEXT( "Present" ) );
	{
		IGpuResource& SwapChainGpuResource = *DCast<IGpuResource*>( pSwapChainSurface );
		UIContext.TransitionResource( SwapChainGpuResource, RS_Present );
	}
	PresentContext.End();

	GetClientViewport().PresentOneFrame();
}

void HEditorEngine::Shutdown()
{
	HEngine::Shutdown();

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

	for (size_t i = 0; i < m_EditorPanels.size(); ++i)
	{
		m_EditorPanels[i]->Initialize();
	}

	m_MenuBar.AddMenuItem( "File", "Exit", this, &HEditorEngine::OnExitMenuItem );
	m_MenuBar.AddMenuItem( "Developer", "Reload Pipeline Shaders", this, &HEditorEngine::OnReloadPipelineShaders );
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

	// Window
	Dispatcher.Dispatch<WindowResizeEvent>( this, &HEditorEngine::OnWindowResized );
	Dispatcher.Dispatch<WindowFocusEvent>( this, &HEditorEngine::OnWindowFocus );
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &HEditorEngine::OnWindowLostFocus );
	Dispatcher.Dispatch<WindowClosedEvent>( this, &HEditorEngine::OnClientWindowClosed );
	Dispatcher.Dispatch<WindowFileDropEvent>( this, &HEditorEngine::OnClientWindowDropFile );
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
	float XOffset = e.GetXOffset() * m_UserPreferences.VerticalScrollSpeedMultiplier;
	float YOffset = e.GetYOffset() * m_UserPreferences.HorizontalScrollSpeedMultiplier;
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

void HEditorEngine::OnExitMenuItem()
{
	RequestShutdown();
}

void HEditorEngine::OnReloadPipelineShaders()
{
	HE_LOG( Log, TEXT( "Starting shader pipeline reload." ) );
	GCommandManager->IdleGPU();
	GetClientViewport().ReloadRenderPipelines();
	HE_LOG( Log, TEXT( "Shader pipeline reload completed." ) );
}
