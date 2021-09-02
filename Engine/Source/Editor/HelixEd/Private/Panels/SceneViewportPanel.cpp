#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "Direct3D12/ColorBufferD3D12.h"
#include "IDevice.h"
#include "Engine/HEngine.h"
#include "Input/RawInput.h"
#include "World/HLevel.h"

#include "Debug/ADebugCharacter.h"
#include "GameFramework/GameInstance.h"
#include "GameFramework/Actor/APlayerCharacter.h"


SceneViewportPanel::SceneViewportPanel()
	: m_DescriptorHandle()
	, m_HandleSize(0)
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
	//m_pDebugPawn->Tick( DeltaTime );

	RawInputSurveyer& Input = GEngine->GetClientViewport().GetInputDispatcher()->GetInputSureyor();
	if (Input.IsPressed( Key_LShift ) && Input.IsFirstPressed( Key_Escape ))
	{
		//GGameInstance->OnGameLostFocus();

		// Set the debug camera.
		//GEngine->GetClientViewport().GetWorld().SetCurrentSceneRenderCamera( m_pDebugPawn->GetCameraComponent() );

		// Unlock the mouse if the game locked it.
		GEngine->GetClientViewport().UnlockMouseFromScreenCenter();
		HE_LOG( Log, TEXT( "Unlocking cursor from scene viewport." ) );
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
		
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			GEngine->GetClientViewport().LockMouseToScreenCenter();
			HE_LOG( Log, TEXT( "Locking cursor to scene viewport." ) );
		}

		ColorBufferD3D12* Buffer = GEngine->GetClientViewport().GetPreDisplayBuffer()->As<ColorBufferD3D12*>();

		ID3D12Device* pDevice = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
		uint32_t DestCount = 1;
		uint32_t SourceCounts[] = { 1 };

		D3D12_CPU_DESCRIPTOR_HANDLE SourceTextures[] =
		{
			Buffer->GetSRVHandle(),
		};

		DescriptorHandle dest = m_DescriptorHandle + 1 * m_HandleSize;

		D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{ dest.GetCpuPtr() };
		pDevice->CopyDescriptors( 1, &CpuHandle, &DestCount, DestCount, SourceTextures, SourceCounts, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

		ImGui::Image( (ImTextureID)dest.GetGpuPtr(), ImGui::GetWindowSize() );
		
	}
	ImGui::End();

}
