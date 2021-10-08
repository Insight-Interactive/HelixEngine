#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CommonStructs.h"

#include "RendererCore.h"

class FRootParameter
{
	friend class RootSignature;
public:

	FRootParameter()
	{
		m_RootParam.ParameterType = (ERootParameterType)0xFFFFFFFF;
	}

	~FRootParameter()
	{
		Clear();
	}

	void Clear()
	{
		if (m_RootParam.ParameterType == RPT_DescriptorTable)
			delete[] m_RootParam.DescriptorTable.pDescriptorRanges;

		m_RootParam.ParameterType = (ERootParameterType)0xFFFFFFFF;
	}

	void InitAsConstants(uint32 Register, uint32 NumDwords, EShaderVisibility Visibility = SV_All, uint32 Space = 0)
	{
		m_RootParam.ParameterType = RPT_32BitConstants;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Constants.Num32BitValues = NumDwords;
		m_RootParam.Constants.ShaderRegister = Register;
		m_RootParam.Constants.RegisterSpace = Space;
	}

	void InitAsConstantBuffer(uint32 Register, EShaderVisibility Visibility = SV_All, uint32 Space = 0)
	{
		m_RootParam.ParameterType = RPT_ConstantBufferView;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = Space;
	}

	void InitAsBufferSRV(uint32 Register, EShaderVisibility Visibility = SV_All, uint32 Space = 0)
	{
		m_RootParam.ParameterType = RPT_ShaderResourceView;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = Space;
	}

	void InitAsBufferUAV(uint32 Register, EShaderVisibility Visibility = SV_All, uint32 Space = 0)
	{
		m_RootParam.ParameterType = RPT_UnorderedAccessView;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = Space;
	}

	void InitAsDescriptorRange(EDescriptorRangeType Type, uint32 Register, uint32 Count, EShaderVisibility Visibility = SV_All, uint32 Space = 0)
	{
		InitAsDescriptorTable(1, Visibility);
		SetTableRange(0, Type, Register, Count, Space);
	}

	void InitAsDescriptorTable(uint32 RangeCount, EShaderVisibility Visibility = SV_All)
	{
		m_RootParam.ParameterType = RPT_DescriptorTable;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.DescriptorTable.NumDescriptorRanges = RangeCount;
		m_RootParam.DescriptorTable.pDescriptorRanges = new FDescriptorRange[RangeCount];
	}

	void SetTableRange(uint32 RangeIndex, EDescriptorRangeType Type, uint32 Register, uint32 Count, uint32 Space = 0)
	{
		FDescriptorRange* range = const_cast<FDescriptorRange*>(m_RootParam.DescriptorTable.pDescriptorRanges + RangeIndex);
		ZeroMemory(range, sizeof(FDescriptorRange));
		range->Type = Type;
		range->NumDescriptors = Count;
		range->BaseShaderRegister = Register;
		range->RegisterSpace = Space;
		range->OffsetInDescriptorsFromTableStart = HE_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	const FRootParameterDesc& operator() (void) const { return m_RootParam; }


protected:

	FRootParameterDesc m_RootParam;
};

namespace DX12
{
	class D3D12DynamicDescriptorHeap;
}

// Maximum 64 DWORDS divied up amongst all root parameters.
// Root constants = 1 DWORD * NumConstants
// Root descriptor (CBV, SRV, or UAV) = 2 DWORDs each
// Descriptor table pointer = 1 DWORD
// Static samplers = 0 DWORDS (compiled into shader)
class FRootSignature
{
	friend class FDynamicDescriptorHeapD3D12;

public:

	FRootSignature(uint32 NumRootParams = 0, uint32 NumStaticSamplers = 0)
		: m_Finalized(false)
		, m_NumParameters(NumRootParams)
	{
		Reset(NumRootParams, NumStaticSamplers);
	}

	~FRootSignature()
	{
	}

	virtual void DestroyAll(void) = 0;

	virtual void* GetNativeSignature() = 0;

	virtual void Initialize(const FRootSignatureDesc& Desc) = 0;

	void Reset(uint32 NumRootParams, uint32 NumStaticSamplers = 0)
	{
		if (NumRootParams > 0)
			m_ParamArray.reset(new FRootParameter[NumRootParams]);
		else
			m_ParamArray = nullptr;
		m_NumParameters = NumRootParams;

		if (NumStaticSamplers > 0)
			m_SamplerArray.reset(new FStaticSamplerDesc[NumStaticSamplers]);
		else
			m_SamplerArray = nullptr;
		m_NumSamplers = NumStaticSamplers;
		m_NumInitializedStaticSamplers = 0;
	}

	FRootParameter& operator[] (size_t EntryIndex)
	{
		HE_ASSERT(EntryIndex < m_NumParameters);
		return m_ParamArray.get()[EntryIndex];
	}

	const FRootParameter& operator[] (size_t EntryIndex) const
	{
		HE_ASSERT(EntryIndex < m_NumParameters);
		return m_ParamArray.get()[EntryIndex];
	}

	void InitStaticSampler(uint32 Register, const FSamplerDesc& NonStaticSamplerDesc,
		EShaderVisibility Visibility = SV_All);

	virtual void Finalize(const WChar* name, ERootSignatureFlags Flags = RSF_None) = 0;

protected:
	bool m_Finalized;
	uint32 m_NumParameters;
	uint32 m_NumSamplers;
	uint32 m_NumInitializedStaticSamplers;
	uint32 m_DescriptorTableBitMap;		// One bit is set for root parameters that are non-sampler descriptor tables.
	uint32 m_SamplerTableBitMap;		// One bit is set for root parameters that are sampler descriptor tables.
	uint32 m_DescriptorTableSize[16];	// Non-sampler descriptor tables need to know their descriptor count.
	std::unique_ptr<FRootParameter[]> m_ParamArray;
	std::unique_ptr<FStaticSamplerDesc[]> m_SamplerArray;

};
