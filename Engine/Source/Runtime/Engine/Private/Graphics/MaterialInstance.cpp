#include "EnginePCH.h"

//#include "Renderer/MaterialInstance.h"
//
//#include "CommandContext.h"
//
//
//HMaterialInstance::HMaterialInstance()
//{
//
//}
//
//HMaterialInstance::~HMaterialInstance()
//{
//	m_ConstBuffersMappings.clear();
//	m_TextureMappings.clear();
//	for (auto& Iter : m_ConstBufferVarMappings)
//	{
//		for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
//		{
//			if (Iter.second[i].first)
//			{
//				delete Iter.second[i].first; // Failed because multiple entries in m_ConstBufferVarMappings can reference the same const buffer memory location.
//				Iter.second[i].first = nullptr;
//			}
//		}
//	}
//}
//
//void HMaterialInstance::Bind(FCommandContext& GfxContext)
//{
//	Super::Bind(GfxContext);
//
//	// Bind buffers
//	for (auto& Iter : m_ConstBuffersMappings)
//	{
//		uint32& RootIndex = Iter.second.first;
//		std::vector<FConstantBufferInterface*>& ConstBuffers = Iter.second.second;
//
//		if (ConstBuffers.size() > 0)
//		{
//			FConstantBufferInterface* pBuffer = ConstBuffers[GEngine->GetClientViewport().GetFrameIndex()];
//			if (pBuffer)
//				GfxContext.SetGraphicsConstantBuffer(RootIndex, *pBuffer);
//			else
//				HE_ASSERT(false);
//		}
//	}
//
//	// Bind textures
//	for (auto& Iter : m_TextureMappings)
//	{
//		uint32& RootIndex = Iter.second.first;
//		HTextureRef Texture = Iter.second.second;
//
//		if (Texture.Get())
//			GfxContext.SetTexture(RootIndex, Texture);
//		else
//			HE_ASSERT(false);
//	}
//}
