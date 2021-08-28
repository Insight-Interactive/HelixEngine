#include "HelixEdPCH.h"

#include "Editor/HEditorEngine.h"

#include "RendererCore.h"
#include "IDevice.h"
#include "ICommandContext.h"
#include "IGpuResource.h"
#include "Direct3D12/ColorBufferD3D12.h"
#include "Direct3D12/TextureD3D12.h"
#include "Direct3D12/CommandContextD3D12.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Engine/Event/EngineEvent.h"

#include "ITextureManager.h"
#include "StandaloneRenderer/Technique/DeferredShadingTech.h"
#include "System.h"
#include "StringHelper.h"
#include "Jobs/ThreadPool.h"


HEditorEngine::HEditorEngine()
{
}

HEditorEngine::~HEditorEngine()
{
}

void HEditorEngine::Startup()
{
	HEngine::Startup();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Window Independence
	IO.ConfigWindowsMoveFromTitleBarOnly = true;
	IO.ConfigDockingAlwaysTabBar = true;
	IO.Fonts->AddFontFromFileTTF( "../../Engine/Content/Fonts/Cousine-Regular.ttf", 15.0f );

	EnableDarkMode( true );

	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowRounding = 0.0f;
	Style.Colors[ImGuiCol_WindowBg].w = 1.f;

	void* hwnd = GetClientViewport().GetWindow().GetNativeWindow();
	bool ImGuiImplWin32Succeeded = ImGui_ImplWin32_Init( hwnd );
	HE_ASSERT( ImGuiImplWin32Succeeded );
	SetupImGuiRenderBackend();

	m_SceneViewport.Initialize();
	m_EditorPanels.push_back( &m_SceneViewport );
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

void StandaloneGameLaunch(void*)
{
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( sizeof( Path ), Path );
	String ExePath = StringHelper::UTF16ToUTF8( Path );
	ExePath += "Engine.exe";
	ExePath += " -launchcfg LaunchGame";

	system( ExePath.c_str() );
}
DescriptorHandle m_DescriptorHandle;

void HEditorEngine::RenderClientViewport( float DeltaTime )
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport( 0, ImGuiDockNodeFlags_PassthruCentralNode );

	GetClientViewport().Update( DeltaTime );
	GetClientViewport().Render();

	static bool ShowDemoWindow = true;
	ImGui::ShowDemoWindow( &ShowDemoWindow );


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu( "File" ))
		{
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu( "Edit" ))
		{
			if (ImGui::MenuItem( "Undo", "CTRL+Z" )) {}
			if (ImGui::MenuItem( "Redo", "CTRL+Y", false, false )) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem( "Cut", "CTRL+X" )) {}
			if (ImGui::MenuItem( "Copy", "CTRL+C" )) {}
			if (ImGui::MenuItem( "Paste", "CTRL+V" )) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

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
		for(size_t i = 0; i < m_EditorPanels.size(); ++i)
			m_EditorPanels[i]->Render( Context );
	}
	Context.End();

	//ImGui::Begin( "Scene Viewport" );
	//{

	//	//if (ImGui::IsWindowFocused())
	//	{
	//		//GetMainClientViewPort().GetInputDispatcher()->GetInputSureyor().AcquireMouse();
	//		ICommandContext& UIContext = ICommandContext::Begin( TEXT( "Setup Editor Display" ) );
	//		{
	//			UIContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap );
	//			ColorBufferD3D12* Buffer = GetClientViewport().GetPreDisplayBuffer()->As<ColorBufferD3D12*>();

	//			ID3D12Device* pDevice = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
	//			static bool Created = false;
	//			uint32 HandleSize = 0;
	//			if (!Created)
	//			{
	//				Created = true;
	//				m_DescriptorHandle = GTextureHeap->Alloc( 1 );
	//				HandleSize = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
	//			}

	//			uint32_t DestCount = 1;
	//			uint32_t SourceCounts[] = { 1 };

	//			D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[] =
	//			{
	//				Buffer->GetSRVHandle(),
	//			};

	//			DescriptorHandle dest = m_DescriptorHandle + 1 * HandleSize;

	//			D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{ dest.GetCpuPtr() };
	//			pDevice->CopyDescriptors( 1, &CpuHandle, &DestCount, DestCount, SourceTextures, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	//			ImGui::Image( (ImTextureID)dest.GetGpuPtr(), ImGui::GetWindowSize() );
	//		}
	//		UIContext.End();
	//	}
	//	/*else
	//	{
	//		GetMainClientViewPort().GetInputDispatcher()->GetInputSureyor().UnacquireMouse();
	//	}*/
	//}
	//ImGui::End();

	ImGui::Render();


	IColorBuffer* pSwapChainSurface = GetClientViewport().GetWindow().GetRenderSurface();

	ICommandContext& UIContext = ICommandContext::Begin( TEXT( "Draw Editor" ) );
	{
		// TODO: Doesnt't work for some reason. Only works in ViewportContext.cpp
		//IGpuResource& SwapChainGpuResource = *DCast<IGpuResource*>( pSwapChainSurface );
		//UIContext.TransitionResource( SwapChainGpuResource, RS_RenderTarget );

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

	// Window
	Dispatcher.Dispatch<WindowResizeEvent>( this, &HEditorEngine::OnWindowResized );
	Dispatcher.Dispatch<WindowFocusEvent>( this, &HEditorEngine::OnWindowFocus );
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &HEditorEngine::OnWindowLostFocus );
	Dispatcher.Dispatch<WindowClosedEvent>( this, &HEditorEngine::OnClientWindowClosed );
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

