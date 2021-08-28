#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IPixelBuffer.h"
#include "GpuResourceD3D12.h"

#include "CommonEnums.h"
#include "CommonStructs.h"
#include "../D3DCommon/D3DCommon.h"

class IDevice;


class RENDER_API PixelBufferD3D12 : public IPixelBuffer, public GpuResourceD3D12
{
public:
	PixelBufferD3D12()
	{
	}
	virtual ~PixelBufferD3D12()
	{
	}

	virtual ResourceDesc DescribeTex2D(uint32 Width, uint32 Height, uint32 DepthOrArraySize, uint32 NumMips, EFormat Format, uint32 Flags) override;
	virtual void AssociateWithResource(IDevice* Device, const WChar* Name, void* Resource, EResourceState CurrentState) override;
	virtual void CreateTextureResource(IDevice* Device, const WChar* Name, const ResourceDesc& ResourceDesc, const ClearValue& ClearValue) override;

};
