#pragma once

#include "CoreFwd.h"
#include "CommonEnums.h"


struct ViewPort
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};

struct Rect
{
	int32 Left;
	int32 Top;
	int32 Right;
	int32 Bottom;
};

struct SampleDesc
{
	uint32 Count;
	uint32 Quality;
};

struct MipRegion
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
};

struct ResourceDesc
{
	EResourceDimension Dimension;
	uint64 Alignment;
	uint64 Width;
	uint32 Height;
	uint16 DepthOrArraySize;
	uint16 MipLevels;
	EFormat Format;
	SampleDesc SampleDesc;
	ETextureLayout Layout;
	EResourceFlags Flags;
	MipRegion SamplerFeedbackMipRegion;
};

struct SwapChainDescription
{
	uint32 Width;
	uint32 Height;
	uint32 BufferCount;
	SampleDesc SampleDesc;
	EFormat Format;
	void* NativeWindow;
};

struct DepthStencilValue
{
	float Depth;
	uint8 Stencil;
};

struct ClearValue
{
	EFormat Format;
	float Color[4];
	DepthStencilValue DepthStencil;
};

struct ShaderByteCode
{
	const void* pShaderByteCode;
	uint64 ByteCodeLength;
};

struct RenderTargetBlendDesc
{
	bool BlendEnable;
	bool LogicOpEnable;
	EBlend SourceBlend;
	EBlend DestBlend;
	EBlendOp BlendOp;
	EBlend SourceBlendAlpha;
	EBlend DestBlendAlpha;
	EBlendOp BlendOpAlpha;
	ELogicOp LocigOp;
	uint8 RenderTargetWriteMask;
};

struct SubResourceData
{
	const void* pData;
	uint64 RowPitch;
	uint64 SlicePitch;
};

struct BlendDesc
{
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	RenderTargetBlendDesc  RenderTarget[8];
};

struct StencilOpDesc
{
	EStencilOp StencilFailOp;
	EStencilOp StencilDepthFailOp;
	EStencilOp StencilPassOp;
	EComparisonFunc StencilFunc;
};

struct DepthStencilStateDesc
{
	bool DepthEnable;
	EDepthWriteMask DepthWriteMask;
	EComparisonFunc DepthFunc;
	bool StencilEnabled;
	uint8 StencilReadMask;
	uint8 StencilWriteMask;
	StencilOpDesc FrontFace;
	StencilOpDesc BackFace;

};

struct RasterizerDesc
{
	EFillMode FillMode;
	ECullMode CullMode;
	bool FrontCounterClockwise;
	uint32 DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;
	bool DepthClipEnabled;
	bool MultiSampleEnable;
	bool AntialiazedLineEnabled;
	uint32 ForcedSampleCount;
	EConseritiveRasterMode ConservativeRaster;
};

struct InputElementDesc
{
	char* SemanticName;
	uint32 SemanticIndex;
	EFormat Format;
	uint32 InputSlot;
	uint32 AlignedByteOffset;
	EInputClassification InputSlotClass;
	uint32 InstanceDataStepRate;
};

struct InputLayoutDesc
{
	const InputElementDesc* pInputElementDescs;
	uint32 NumElements;
};

struct PipelineStateDesc
{
	class IRootSignature* pRootSignature;
	ShaderByteCode VertexShader;
	ShaderByteCode PixelShader;
	ShaderByteCode DomainShader;
	ShaderByteCode HullShader;
	ShaderByteCode GeometryShader;
	BlendDesc BlendState;
	uint32 SampleMask;
	RasterizerDesc RasterizerDesc;
	DepthStencilStateDesc DepthStencilState;
	InputLayoutDesc InputLayout;
	EIndexBufferStripCutValue IBStripCutValue;
	EPrimitiveTopologyType PrimitiveTopologyType;
	uint32 NumRenderTargets;
	EFormat RTVFormats[8];
	EFormat DSVFormat;
	SampleDesc SampleDesc;
	uint32 NodeMask;
	void* CachedPSO;
	EPipelineStateFlags Flags;
};

struct DescriptorRange
{
	EDescriptorRangeType Type;
	uint32 NumDescriptors;
	uint32 BaseShaderRegister;
	uint32 RegisterSpace;
	uint32 OffsetInDescriptorsFromTableStart;
};

struct RootDescriptorTable
{
	uint32 NumDescriptorRanges;
	const DescriptorRange* pDescriptorRanges;
};

struct RootConstants
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	uint32 Num32BitValues;
};

struct RootDescriptor
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
};

struct SamplerDesc
{
	EFilter Filter;
	ETextureAddressMode AddressU;
	ETextureAddressMode AddressV;
	ETextureAddressMode AddressW;
	float MipLODBias;
	uint32 MaxAnisotropy;
	EComparisonFunc ComparisonFunc;
	float BorderColor[4];
	float MinLOD;
	float MaxLOD;
};

struct StaticSamplerDesc
{
	EFilter Filter;
	ETextureAddressMode AddressU;
	ETextureAddressMode AddressV;
	ETextureAddressMode AddressW;
	float MipLODBias;
	uint32 MaxAnisotropy;
	EComparisonFunc ComparisonFunc;
	EStaticBorderColor BorderColor;
	float MinLOD;
	float MaxLOD;
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	EShaderVisibility ShaderVisibility;
};

struct RootParameter
{
	ERootParameterType ParameterType;
	union
	{
		RootDescriptorTable DescriptorTable;
		RootConstants Constants;
		RootDescriptor Descriptor;
	};
	EShaderVisibility ShaderVisibility;
};

struct RootSignatureDesc
{
	uint32 NumParams;
	const RootParameter* pParameters;
	uint32 NumStaticSamplers;
	const StaticSamplerDesc* pStaticSamplers;
	ERootSignatureFlags Flags;
};

struct CpuDescriptorHandle
{
	uint64 Ptr;
};

struct GpuDescriptorHandle
{
	uint64 Ptr;
};

struct Blob
{

};