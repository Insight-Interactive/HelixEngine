#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "ColorBuffer.h"
#include "RenderDevice.h"
#include "Engine/Engine.h"
#include "Input/RawInput.h"
#include "World/Level.h"
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
	m_DescriptorHandle = GTextureHeap.Alloc( 1 );
	ID3D12Device* pDevice = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
	m_HandleSize = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	FActorInitArgs InitArgs{ &GetOwningViewport()->GetWorld(), TEXT( "Editor Debug Pawn" ) };
	m_pDebugPawn = new ADebugPawn( InitArgs );
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
		// Notify the game the "window"(the scene viewport) lost focus.
		GGameInstance->OnGameLostFocus();

		UnFreezeDebugCamera();

		// Unlock the mouse if the game locked it.
		GetOwningViewport()->UnlockMouseFromScreenCenter();
		GetOwningViewport()->GetInputDispatcher()->SetCanDispatchListeners( false );
	}
}

void SceneViewportPanel::Render( FCommandContext& CmdCtx )
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

			HWND* Window = (HWND*)GetOwningViewport()->GetWindow().GetNativeWindow();
			RECT rcClip;           // new area for ClipCursor
			RECT rcOldClip;        // previous area for ClipCursor

			::GetClipCursor( &rcOldClip );
			
			::GetWindowRect( *Window, &rcClip );

			::ClipCursor( &rcClip );

			::ClipCursor( &rcOldClip );
		}
		// Don't stop rotating until the user lets go of the mouse button.
		m_IsCameraRotating = ImGui::IsMouseDown( ImGuiMouseButton_Right );
		m_pDebugPawn->SetCanRotateCamera( m_IsCameraRotating );

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Left ) && GEngine->IsPlayingInEditor())
		{
			GetOwningViewport()->GetInputDispatcher()->SetCanDispatchListeners( true );
			GGameInstance->OnGameSetFocus();
		}

		// Render the scene to the viewport.
		//
		FColorBuffer* Buffer = GetOwningViewport()->GetPreDisplayBuffer();
		ID3D12Device* pDevice = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
		uint32_t DestCount = 1;
		uint32_t SourceCounts[] = { 1 };
		D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[] =
		{
			Buffer->GetSRVHandle(),
		};
		FDescriptorHandle dest = m_DescriptorHandle;
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
