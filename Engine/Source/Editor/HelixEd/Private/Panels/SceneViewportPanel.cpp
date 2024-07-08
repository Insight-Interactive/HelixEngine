#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "ColorBuffer.h"
#include "RenderDevice.h"
#include "Engine/Engine.h"
#include "Input/RawInput.h"
#include "World/Level.h"
#include "Engine/ViewportContext.h"
#include "GameFramework/Actor/ADebugPawn.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/Actor/ACharacter.h"
#include "GameFramework/Components/HFirstPersonCameraComponent.h"
#include "Engine/Event/EngineEvent.h"
#include "Input/MouseEvent.h"


extern HGameInstance* GGameInstance;


SceneViewportPanel::SceneViewportPanel()
	: m_DescriptorHandle()
	, m_IsCameraRotating( false )
{
}

SceneViewportPanel::~SceneViewportPanel()
{
}

void SceneViewportPanel::Initialize( FViewportContext* pOwningTab )
{
	Super::Initialize( pOwningTab );

	m_DescriptorHandle = GTextureHeap.Alloc( 1 );
	FActorInitArgs InitArgs{ &GetOwningViewport().GetWorld(), "Editor Debug Pawn", true, true };
	m_pDebugPawn = new ADebugPawn( InitArgs );
	m_pDebugPawn->BeginPlay();

	ActivateDebugCamera();
}

void SceneViewportPanel::UnInitialize()
{

}

void SceneViewportPanel::Tick( float DeltaTime ) 
{
	//if(m_IsCameraRotating)
		m_pDebugPawn->Tick( DeltaTime );

	if (GEngine->IsPlayingInEditor())
	{
		if (GetOwningViewport().IsPressed( Key_LShift ) && GetOwningViewport().IsFirstPressed( Key_Escape ))
		{
			// Notify the game the "window"(the scene viewport) lost focus.
			//GGameInstance->OnGameLostFocus();

			UnFreezeDebugCamera();

			// Unlock the mouse if the game locked it.
			GetOwningViewport().ShowMouse();
			GetOwningViewport().UnlockMouseFromScreenCenter();
			GetOwningViewport().GetInputDispatcher()->SetCanDispatchListeners( false );
		}
	}
}

void SceneViewportPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Scene Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
		// Handle debug camera controls
		// Only rotate the camera if the mouse is over the scene viewport.
		//
		if (!GEngine->IsPlayingInEditor() && ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Right ))
		{
			if (!m_IsCameraRotating)
			{
				m_IsCameraRotating = true;
				GetOwningViewport().HideMouse();
				GetOwningViewport().LockMouseToScreenCenter();

				m_pDebugPawn->SetCanRotateCamera( true );
			}
		}
		else
		{
			if (!GEngine->IsPlayingInEditor() && !ImGui::IsMouseDown( ImGuiMouseButton_Right ))
			{
				if (m_IsCameraRotating)
				{
					m_IsCameraRotating = false;
					GetOwningViewport().ShowMouse();
					GetOwningViewport().UnlockMouseFromScreenCenter();

					m_pDebugPawn->SetCanRotateCamera( false );
				}
			}
		}

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
	GetOwningViewport().GetWorld().SetCurrentSceneRenderCamera( (HCameraComponent*)m_pDebugPawn->GetCameraComponent() );
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
	Dispatcher.Dispatch<EngineBeginPlayEvent>( this, &SceneViewportPanel::OnAppBeginPlay );
	Dispatcher.Dispatch<EngineEndPlayEvent>( this, &SceneViewportPanel::OnAppEndPlay );

	Dispatcher.Dispatch<MouseButtonPressedEvent>( this, &SceneViewportPanel::OnMouseButtonPressed );
	Dispatcher.Dispatch<MouseButtonReleasedEvent>( this, &SceneViewportPanel::OnMouseButtonReleased );
}

bool SceneViewportPanel::OnAppBeginPlay( EngineBeginPlayEvent& e )
{
	return false;
}

bool SceneViewportPanel::OnAppEndPlay( EngineEndPlayEvent& e )
{
	return false;
}


bool SceneViewportPanel::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	return false;
}

bool SceneViewportPanel::OnMouseButtonReleased( MouseButtonReleasedEvent& e )
{
	return false;
}
