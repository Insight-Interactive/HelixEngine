#pragma once

#include "CoreFwd.h"
#include "CommonEnums.h"


struct FViewPort
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};

struct FRect
{
	int32 Left;
	int32 Top;
	int32 Right;
	int32 Bottom;
};

struct FSampleDesc
{
	uint32 Count;
	uint32 Quality;
};

struct FMipRegion
{
	uint32 Width;
	uint32 Height;
	uint32 Depth;
};

struct FResourceDesc
{
	EResourceDimension Dimension;
	uint64 Alignment;
	uint64 Width;
	uint32 Height;
	uint16 DepthOrArraySize;
	uint16 MipLevels;
	EFormat Format;
	FSampleDesc SampleDesc;
	ETextureLayout Layout;
	EResourceFlags Flags;
	FMipRegion SamplerFeedbackMipRegion;
};

struct FSwapChainDesc
{
	uint32 Width;
	uint32 Height;
	FSampleDesc SampleDesc;
	ESwapchainBitDepth BackBufferColorDepth;
	void* NativeWindow;
};

struct FDepthStencilValue
{
	float Depth;
	uint8 Stencil;
};

struct FClearValue
{
	EFormat Format;
	float Color[4];
	FDepthStencilValue DepthStencil;
};

struct FShaderByteCode
{
	const void* pShaderByteCode;
	uint64 ByteCodeLength;
};

struct FRenderTargetBlendDesc
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

struct FSubResourceData
{
	const void* pData;
	uint64 RowPitch;
	uint64 SlicePitch;
};

struct FBlendDesc
{
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	FRenderTargetBlendDesc  RenderTarget[8];
};

struct FStencilOpDesc
{
	EStencilOp StencilFailOp;
	EStencilOp StencilDepthFailOp;
	EStencilOp StencilPassOp;
	EComparisonFunc StencilFunc;
};

struct FDepthStencilStateDesc
{
	bool DepthEnable;
	EDepthWriteMask DepthWriteMask;
	EComparisonFunc DepthFunc;
	bool StencilEnabled;
	uint8 StencilReadMask;
	uint8 StencilWriteMask;
	FStencilOpDesc FrontFace;
	FStencilOpDesc BackFace;
};

struct FRasterizerDesc
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

struct FInputElementDesc
{
	char* SemanticName;
	uint32 SemanticIndex;
	EFormat Format;
	uint32 InputSlot;
	uint32 AlignedByteOffset;
	EInputClassification InputSlotClass;
	uint32 InstanceDataStepRate;
};

struct FInputLayoutDesc
{
	const FInputElementDesc* pInputElementDescs;
	uint32 NumElements;
};

struct FPipelineStateDesc
{
	class FRootSignature* pRootSignature;
	FShaderByteCode VertexShader;
	FShaderByteCode PixelShader;
	FShaderByteCode DomainShader;
	FShaderByteCode HullShader;
	FShaderByteCode GeometryShader;
	FBlendDesc BlendState;
	uint32 SampleMask;
	FRasterizerDesc RasterizerDesc;
	FDepthStencilStateDesc DepthStencilState;
	FInputLayoutDesc InputLayout;
	EIndexBufferStripCutValue IBStripCutValue;
	EPrimitiveTopologyType PrimitiveTopologyType;
	uint32 NumRenderTargets;
	EFormat RTVFormats[8];
	EFormat DSVFormat;
	FSampleDesc SampleDesc;
	uint32 NodeMask;
	void* CachedPSO;
	EPipelineStateFlags Flags;
};

struct FDescriptorRange
{
	EDescriptorRangeType Type;
	uint32 NumDescriptors;
	uint32 BaseShaderRegister;
	uint32 RegisterSpace;
	uint32 OffsetInDescriptorsFromTableStart;
};

struct FRootDescriptorTable
{
	uint32 NumDescriptorRanges;
	const FDescriptorRange* pDescriptorRanges;
};

struct FRootConstants
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	uint32 Num32BitValues;
};

struct FRootDescriptor
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
};

struct FSamplerDesc
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

struct FStaticSamplerDesc
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

struct FRootParameterDesc
{
	ERootParameterType ParameterType;
	union
	{
		FRootDescriptorTable DescriptorTable;
		FRootConstants Constants;
		FRootDescriptor Descriptor;
	};
	EShaderVisibility ShaderVisibility;
};

struct FRootSignatureDesc
{
	uint32 NumParams;
	const FRootParameterDesc* pParameters;
	uint32 NumStaticSamplers;
	const FStaticSamplerDesc* pStaticSamplers;
	ERootSignatureFlags Flags;
};

struct FCpuDescriptorHandle
{
	uint64 Ptr;
};

struct FGpuDescriptorHandle
{
	uint64 Ptr;
};

struct FResolution
{
	uint32 Width;
	uint32 Height;
};


