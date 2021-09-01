#include "HelixEdPCH.h"

#include "Panels/SceneViewportPanel.h"

#include "Direct3D12/ColorBufferD3D12.h"
#include "IDevice.h"
#include "Engine/HEngine.h"


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
}

void SceneViewportPanel::UnInitialize()
{

}

void SceneViewportPanel::Tick( float DeltaTime ) 
{

}

void SceneViewportPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Scene Viewport" );
	{
		//if (ImGui::IsWindowFocused())
		{
			//GetMainClientViewPort().GetInputDispatcher()->GetInputSureyor().AcquireMouse();
			
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
		/*else
		{
			GetMainClientViewPort().GetInputDispatcher()->GetInputSureyor().UnacquireMouse();
		}*/
	}
	ImGui::End();

}
