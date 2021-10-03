#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "Direct3D12/ColorBufferD3D12.h"
#include "IDevice.h"
#include "Engine/HEngine.h"
#include "Input/RawInput.h"
#include "World/HLevel.h"
#include "Engine/ViewportContext.h"
#include "Developer/ADebugPawn.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/Actor/APlayerCharacter.h"
#include "GameFramework/Components/HCameraComponent.h"


SceneViewportPanel::SceneViewportPanel()
	: m_DescriptorHandle()
	, m_HandleSize(0)
	, m_IsCameraRotating( false )
{
}

SceneViewportPanel::~SceneViewportPanel()
{
}

void SceneViewportPanel::Initialize()
{
	m_DescriptorHandle = GTextureHeap->Alloc( 1 );
	ID3D12Device* pDevice = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
	m_HandleSize = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	m_pDebugPawn = new ADebugPawn( &GetOwningViewport()->GetWorld(), TEXT("Editor Debug Pawn"));
	m_pDebugPawn->BeginPlay();
	
	ActivateDebugCamera();
}

void SceneViewportPanel::UnInitialize()
{

}

void SceneViewportPanel::Tick( float DeltaTime ) 
{
	m_DeltaTime = DeltaTime;
	m_pDebugPawn->Tick( DeltaTime );

	if (GetOwningViewport()->IsPressed( Key_LShift ) && GetOwningViewport()->IsFirstPressed( Key_Escape ))
	{
		//GGameInstance->OnGameLostFocus();

		UnFreezeDebugCamera();

		// Unlock the mouse if the game locked it.
		GetOwningViewport()->UnlockMouseFromScreenCenter();
	}
}

void SceneViewportPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Scene Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
		// Handle debug camera controls
		// Only rotate the camera if the mouse is over the scene viewport.
		//
		HCameraComponent* pCamera = m_pDebugPawn->GetCameraComponent();
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Right ) || m_IsCameraRotating)
		{
			m_IsCameraRotating = true;
		}
		// Dont stop rotating until the user lets go of the mouse button.
		m_IsCameraRotating = ImGui::IsMouseDown( ImGuiMouseButton_Right );
		m_pDebugPawn->SetCanRotateCamera( m_IsCameraRotating );


		// Render the scene to the viewport.
		//
		ColorBufferD3D12* Buffer = GetOwningViewport()->GetPreDisplayBuffer()->As<ColorBufferD3D12*>();
		ID3D12Device* pDevice = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
		uint32_t DestCount = 1;
		uint32_t SourceCounts[] = { 1 };
		D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[] =
		{
			Buffer->GetSRVHandle(),
		};
		DescriptorHandle dest = m_DescriptorHandle;// + 1 * m_HandleSize;
		D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{ dest.GetCpuPtr() };
		pDevice->CopyDescriptors( 1, &CpuHandle, &DestCount, DestCount, SourceTextures, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		ImGui::Image( (ImTextureID)dest.GetGpuPtr(), ImGui::GetWindowSize() );
	}
	ImGui::End();

}

void SceneViewportPanel::ActivateDebugCamera()
{
	GetOwningViewport()->GetWorld().SetCurrentSceneRenderCamera( m_pDebugPawn->GetCameraComponent() );
	UnFreezeDebugCamera();
}

void SceneViewportPanel::DeactivateDebugCamera()
{
	FreezeDebugCamera();
}

void SceneViewportPanel::FreezeDebugCamera()
{
	m_pDebugPawn->SetCanMove( false );
}

void SceneViewportPanel::UnFreezeDebugCamera()
{
	m_pDebugPawn->SetCanMove( true );
}
