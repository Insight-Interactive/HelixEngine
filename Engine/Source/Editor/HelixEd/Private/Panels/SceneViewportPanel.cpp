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
#include "Engine/Event/EngineEvent.h"


SceneViewportPanel::SceneViewportPanel()
	: m_DescriptorHandle()
	, m_IsCameraRotating( false )
	, m_ShouldIgnoreInput( false )
{
}

SceneViewportPanel::~SceneViewportPanel()
{
}

void SceneViewportPanel::Initialize( FViewportContext* pOwningTab )
{
	Super::Initialize( pOwningTab );

	m_DescriptorHandle = GTextureHeap.Alloc( 1 );
	FActorInitArgs InitArgs{ &GetOwningViewport().GetWorld(), TEXT( "Editor Debug Pawn" ), true };
	m_pDebugPawn = new ADebugPawn( InitArgs );
	m_pDebugPawn->BeginPlay();
	
	ActivateDebugCamera();
}

void SceneViewportPanel::UnInitialize()
{

}

void SceneViewportPanel::Tick( float DeltaTime ) 
{
	if(m_IsCameraRotating)
		m_pDebugPawn->Tick( DeltaTime );

	if (GetOwningViewport().IsPressed( Key_LShift ) && GetOwningViewport().IsFirstPressed( Key_Escape ))
	{
		// Notify the game the "window"(the scene viewport) lost focus.
		GGameInstance->OnGameLostFocus();

		UnFreezeDebugCamera();

		// Unlock the mouse if the game locked it.
		GetOwningViewport().UnlockMouseFromScreenCenter();
		GetOwningViewport().GetInputDispatcher()->SetCanDispatchListeners( false );
	}
}

void SceneViewportPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Scene Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
		if (!m_ShouldIgnoreInput)
		{
			// Handle debug camera controls
			// Only rotate the camera if the mouse is over the scene viewport.
			//
			HCameraComponent* pCamera = m_pDebugPawn->GetCameraComponent();
			if (ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Right ) || m_IsCameraRotating)
			{
				m_IsCameraRotating = true;
				GetOwningViewport().LockMouseToScreenCenter();
			}
			// Don't stop rotating until the user lets go of the mouse button.
			m_IsCameraRotating = ImGui::IsMouseDown( ImGuiMouseButton_Right );
			m_pDebugPawn->SetCanRotateCamera( m_IsCameraRotating );

			if (!m_IsCameraRotating)
			{
				GetOwningViewport().UnlockMouseFromScreenCenter();
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Left ) && GEngine->IsPlayingInEditor())
			{
				GetOwningViewport().GetInputDispatcher()->SetCanDispatchListeners( true );
				GGameInstance->OnGameSetFocus();
			}
		}

		GetOwningViewport().GetWorld().GetScene().WaitForRenderingFinished();

		// Get the render target image and present it into the viewport.
		//
		FColorBuffer* Buffer = GetOwningViewport().GetPreDisplayBuffer();
		ID3D12Device* pDevice = (ID3D12Device*)GGraphicsDevice.GetNativeDevice();
		uint32_t DestCount = 1;
		uint32_t SourceCounts[] = { 1 };
		D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[] = { Buffer->GetSRVHandle() };
		FDescriptorHandle Dest = m_DescriptorHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{ Dest.GetCpuPtr() };
		pDevice->CopyDescriptors( 1, &CpuHandle, &DestCount, DestCount, SourceTextures, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		ImGui::Image( (ImTextureID)Dest.GetGpuPtr(), ImGui::GetWindowSize() );
	}
	ImGui::End();

}

void SceneViewportPanel::ActivateDebugCamera()
{
	GetOwningViewport().GetWorld().SetCurrentSceneRenderCamera( m_pDebugPawn->GetCameraComponent() );
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

// Event Processing
//

void SceneViewportPanel::OnEvent( Event& e ) 
{
	EventDispatcher Dispatcher( e );

	// Editor
	Dispatcher.Dispatch<AppBeginPlayEvent>( this, &SceneViewportPanel::OnAppBeginPlay );
	Dispatcher.Dispatch<AppEndPlayEvent>( this, &SceneViewportPanel::OnAppEndPlay );
}

bool SceneViewportPanel::OnAppBeginPlay( AppBeginPlayEvent& e )
{
	// If the app is simulating let the simulation take full advantage of the input.
	m_ShouldIgnoreInput = true;

	return false;
}

bool SceneViewportPanel::OnAppEndPlay( AppEndPlayEvent& e )
{
	m_ShouldIgnoreInput = false;
	
	return false;
}
