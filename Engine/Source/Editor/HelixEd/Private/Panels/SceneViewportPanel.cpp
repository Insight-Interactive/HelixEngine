#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "Direct3D12/ColorBufferD3D12.h"
#include "IDevice.h"
#include "Engine/HEngine.h"
#include "Input/RawInput.h"
#include "World/HLevel.h"
#include "Engine/ViewportContext.h"
#include "Debug/ADebugCharacter.h"
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

	//m_pDebugPawn = GEngine->GetClientViewport().GetWorld().GetCurrentLevel()->CreateActor<ADebugCharacter>();
	//m_pDebugPawn->BeginPlay();
	//
	//GEngine->GetClientViewport().GetWorld().SetCurrentSceneRenderCamera( m_pDebugPawn->GetCameraComponent() );
}

void SceneViewportPanel::UnInitialize()
{

}

void SceneViewportPanel::Tick( float DeltaTime ) 
{
	m_DeltaTime = DeltaTime;
	//m_pDebugPawn->Tick( DeltaTime );

	RawInputSurveyer& Input = GEngine->GetClientViewport().GetInputDispatcher()->GetInputSureyor();
	if (Input.IsPressed( Key_LShift ) && Input.IsFirstPressed( Key_Escape ))
	{
		//GGameInstance->OnGameLostFocus();

		// Set the debug camera.
		//GEngine->GetClientViewport().GetWorld().SetCurrentSceneRenderCamera( m_pDebugPawn->GetCameraComponent() );

		// Unlock the mouse if the game locked it.
		GEngine->GetClientViewport().UnlockMouseFromScreenCenter();
	}
}

void SceneViewportPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Scene Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	{
		//if ( ImGui::IsWindowFocused() && GEngine->IsPlayingInEditor() )
		//{
		//	// TODO if Game simulation is active do the following...
		//	//GGameInstance->OnGameSetFocus();
		//	HWorld& GameWorld = GEngine->GetClientViewport().GetWorld();
		//	GameWorld.SetCurrentSceneRenderCamera( GameWorld.GetPlayerCharacter(0)->GetCameraComponent() );
		//}

		// Only rotate the camera if the mouse is over the scene viewport.
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown( ImGuiMouseButton_Right ) || m_IsCameraRotating)
		{
			float DT = GEngine->GetDeltaTime();
			m_IsCameraRotating = true;
			APlayerCharacter* pClient = GEngine->GetClientViewport().GetWorld().GetPlayerCharacter( 0 );
			HCameraComponent* pCamera = pClient->GetCameraComponent();
			float X = GEngine->GetClientViewport().GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaX() * m_DeltaTime;
			float Y = GEngine->GetClientViewport().GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaY() * m_DeltaTime;
			GEngine->GetClientViewport().GetWorld().GetPlayerCharacter( 0 )->GetCameraComponent()->GetTransform().Rotate( Y, X, 0.f );
			pClient->GetTransform().SetRotationMatrix(
				GEngine->GetClientViewport().GetWorld().GetPlayerCharacter( 0 )->GetCameraComponent()->GetTransform().GetRotationMatrix()
			);
			//FVector3 CharacterPos = pClient->GetTransform().GetPosition();
			//FVector3 CameraPos = pCamera->GetTransform().GetPosition();
			//HE_LOG( Log, TEXT( "Character Pos: %f, %f, %f | Camera Pos: %f, %f, %f" ), CharacterPos.x, CharacterPos.y, CharacterPos.z, CameraPos.x, CameraPos.y, CameraPos.z );
		}
		// Dont stop rotating until the user lets go of the mouse button.
		m_IsCameraRotating = ImGui::IsMouseDown( ImGuiMouseButton_Right );


		ColorBufferD3D12* Buffer = GEngine->GetClientViewport().GetPreDisplayBuffer()->As<ColorBufferD3D12*>();

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
