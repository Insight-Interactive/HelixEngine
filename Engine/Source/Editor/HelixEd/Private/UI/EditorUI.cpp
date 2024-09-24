#include "HelixEdPCH.h"

#include "UI/EditorUI.h"

#include "Engine/GameProject.h"
#include "Engine/Renderer/SwapChain.h"
#include "Engine/ViewportContext.h"
#include "Engine/Renderer/CommandContext.h"


EditorUIContext::EditorUIContext( FViewportContext& Owner )
	: m_pImGuiContext( nullptr )
	, m_Owner( Owner )
{

}

EditorUIContext::~EditorUIContext()
{

}

void EditorUIContext::Setup()
{
	if (IsValid())
	{
		HE_LOG( Warning, TEXT( "This instance of an editor ui context has already been setup!" ) );
		return;
	}

	IMGUI_CHECKVERSION();
	m_pImGuiContext = ImGui::CreateContext();
	HE_ASSERT( m_pImGuiContext != nullptr );
	ImGui::SetCurrentContext( m_pImGuiContext );

	static bool IsIOLoaded = false;
	if (!IsIOLoaded)
	{
		IsIOLoaded = true;
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Window Independence
		IO.ConfigWindowsMoveFromTitleBarOnly = true;
		IO.ConfigDockingAlwaysTabBar = true;
		
		char Path[HE_MAX_PATH];
		FGameProject::GetInstance()->GetContentDirectoryFullPath( "Engine/Fonts/Cousine-Regular.ttf", Path, sizeof( Path ) );
		IO.Fonts->AddFontFromFileTTF( Path, 15.0f );
		IO.IniFilename = NULL;
	}

	// TODO: Make .ini file for each kind of asset editor.
	//String ImGuiIniPath = FGameProject::GetInstance()->GetConfigFolder() + "/imgui.ini";
	//ImGui::LoadIniSettingsFromDisk( ImGuiIniPath.c_str() );

	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowRounding = 0.0f;
	Style.Colors[ImGuiCol_WindowBg].w = 1.f;

#if HE_WINDOWS_DESKTOP
	void* hwnd = *(HWND*)m_Owner.GetWindow().GetNativeWindow();
	bool ImGuiImplWin32Succeeded = ImGui_ImplWin32_Init( hwnd );
	HE_ASSERT( ImGuiImplWin32Succeeded );
#endif // HE_WINDOWS_DESKTOP

	SetupImGuiRenderBackend();
}

void EditorUIContext::Shutdown()
{
	ImGui::DestroyContext( m_pImGuiContext );
}

bool EditorUIContext::IsValid() const
{
	return m_pImGuiContext != nullptr;
}

void EditorUIContext::BeginNewFrame()
{
	ImGui::SetCurrentContext( m_pImGuiContext );

#if R_WITH_D3D12
	ImGui_ImplDX12_NewFrame();
#endif // R_WITH_D3D12
#if HE_WINDOWS_DESKTOP
	ImGui_ImplWin32_NewFrame();
#endif // HE_WINDOWS_DESKTOP
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport( nullptr, ImGuiDockNodeFlags_PassthruCentralNode );
}

void EditorUIContext::EndFrame( FCommandContext& CmdContext )
{
	ImGui::Render();
#if R_WITH_D3D12
	ImGui_ImplDX12_RenderDrawData( ImGui::GetDrawData(), (ID3D12GraphicsCommandList*)CmdContext.GetNativeContext() );
#endif // R_WITH_D3D12
}

void EditorUIContext::SetupImGuiRenderBackend()
{
	FSwapChain* pClientSwapChain = m_Owner.GetWindow().GetSwapChain();

#if R_WITH_D3D12
	const DXGI_FORMAT SwapChainFmt = (DXGI_FORMAT)pClientSwapChain->GetBackBufferFormat();
	const int NumBackBuffers = pClientSwapChain->GetNumBackBuffers();
	ID3D12Device* pD3D12Device = (ID3D12Device*)GGraphicsDevice.GetNativeDevice();
	const bool ImGuiD3D12Succeeded = ImGui_ImplDX12_Init( pD3D12Device, NumBackBuffers, SwapChainFmt );
	HE_ASSERT( ImGuiD3D12Succeeded );
#endif // R_WITH_D3D12
}