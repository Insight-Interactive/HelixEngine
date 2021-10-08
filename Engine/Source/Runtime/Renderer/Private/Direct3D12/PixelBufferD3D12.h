#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IPixelBuffer.h"
#include "GpuResourceD3D12.h"

#include "CommonEnums.h"
#include "CommonStructs.h"
#include "../D3DCommon/D3DCommon.h"

class FRenderDevice;


class RENDER_API FPixelBufferD3D12 : public FPixelBuffer, public FGpuResourceD3D12
{
public:
	FPixelBufferD3D12()
	{
	}
	virtual ~FPixelBufferD3D12()
	{
	}

	virtual FResourceDesc DescribeTex2D(uint32 Width, uint32 Height, uint32 DepthOrArraySize, uint32 NumMips, EFormat Format, uint32 Flags) override;
	virtual void AssociateWithResource(FRenderDevice* Device, const WChar* Name, void* Resource, EResourceState CurrentState) override;
	virtual void CreateTextureResource(FRenderDevice* Device, const WChar* Name, const FResourceDesc& FResourceDesc, const FClearValue& ClearValue) override;

};
