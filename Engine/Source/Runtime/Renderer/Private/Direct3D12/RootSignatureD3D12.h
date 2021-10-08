#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IRootSignature.h"


class RENDER_API FRootSignatureD3D12 : public FRootSignature
{
public:
	FRootSignatureD3D12();
	virtual ~FRootSignatureD3D12();

	virtual void* GetNativeSignature() override { return RCast<void*>(m_pID3D12RootSignature); }

	virtual void DestroyAll() override;

	virtual void Initialize(const FRootSignatureDesc& Desc) override;
	virtual void Finalize(const WChar* name, ERootSignatureFlags Flags) override;

protected:
	ID3D12RootSignature* m_pID3D12RootSignature;
};
