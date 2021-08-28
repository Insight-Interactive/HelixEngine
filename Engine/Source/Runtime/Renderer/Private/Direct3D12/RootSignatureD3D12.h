#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IRootSignature.h"


class RENDER_API RootSignatureD3D12 : public IRootSignature
{
public:
	RootSignatureD3D12();
	virtual ~RootSignatureD3D12();

	virtual void* GetNativeSignature() override { return RCast<void*>(m_pID3D12RootSignature); }

	virtual void DestroyAll() override;

	virtual void Initialize(const RootSignatureDesc& Desc) override;
	virtual void Finalize(const WChar* name, ERootSignatureFlags Flags) override;

protected:
	ID3D12RootSignature* m_pID3D12RootSignature;
};
