// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonEnums.h"
#include "CommonStructs.h"
#include "DescriptorAllocator.h"


#define HE_SIMULTANEOUS_RENDER_TARGET_COUNT		( 8 )
#define HE_DEFAULT_STENCIL_READ_MASK			( 0xFF )
#define HE_DEFAULT_STENCIL_WRITE_MASK			( 0xFF )
#define HE_DEFAULT_DEPTH_BIAS					( 0 )
#define HE_DEFAULT_DEPTH_BIAS_CLAMP				( 0.f )
#define HE_DEFAULT_SLOPE_SCALED_DEPTH_BIAS		( 0.f )
#define HE_INVALID_VERTEX_BUFFER_HANDLE			( VertexBufferUID )( -1 )
#define HE_INVALID_INDEX_BUFFER_HANDLE			( IndexBufferUID )( -1 )
#define	HE_APPEND_ALIGNED_ELEMENT				( 0xFFFFFFFF )
#define HE_INVALID_CONSTANT_BUFFER_HANDLE		( ConstantBufferUID )( -1 )
#define HE_INVALID_GPU_ADDRESS					( -1 )
#define HE_INVALID_MATERIAL_ID					( -1 )
#define HE_DESCRIPTOR_RANGE_OFFSET_APPEND		( 0xFFFFFFFF )

#define HE_D3D12_RENDER_BACKEND_NAME "Direct3D 12"
#define HE_D3D11_RENDER_BACKEND_NAME "Direct3D 11"

// The maximum number of back buffers a swapchain can have.
#define HE_MAX_SWAPCHAIN_BACK_BUFFERS	3


class FStaticGeometryManager;
class MaterialManager;
class LightManager;

// ----------------------
//	Forward Declarations
// ----------------------
//
// Classes
class FRenderDevice;
class FSwapChain;
class FColorBuffer;
class FDepthBuffer;
class FTexture;
class FGpuResource;
class FPipelineState;
class FRootSignature;
class FVertexBuffer;
class FIndexBuffer;
class FDescriptorHeap;
class FCommandManager;
class FContextManager;
class FCommandContext;
class FConstantBufferManager;
class FGeometryBufferManager;
class FTextureManager;
class FFontManager;
class FConstantBufferInterface;
class FDescriptorHandle;
class FBatchRenderer;


// ------------------
//	Extern Variables
// ------------------
// The render context manages the lifetime of these objects.
// 
// Command context managment overlord.
extern FCommandManager GCommandManager;
// Graphics context management overlord.
extern FContextManager GContextManager;
// Graphics rendering device.
extern FRenderDevice GGraphicsDevice;
// Texture overlord.
extern FTextureManager GTextureManager;
// Font overlord.
extern FFontManager GFontManager;
// Default texture container.
extern FTexture GDefaultTextures[];
// Heap holding all shader visible textures.
extern FDescriptorHeap GTextureHeap;
// Manager of all static mesh geometry in the world.
extern FStaticGeometryManager GStaticGeometryManager;
// Common resolution formats an application may use.
extern const FResolution GCommonResolutions[];


// ----------
//	Typedefs
// ----------

// UIDs
//

// Handle to vertex buffer instance in the FGeometryBufferManager.
typedef uint32 VertexBufferUID;
// Handle to index buffer instance in the FGeometryBufferManager.
typedef uint32 IndexBufferUID;
// Handle to constant buffer instance in the FConstantBufferManager.
typedef uint32 ConstantBufferUID;


// -----------------
//	Utility Methods
// -----------------
//
/*
	Returns a default texture given an enum value.
*/
FTexture& GetDefaultTexture(EDefaultTexture TexID);

#if R_WITH_D3D12
extern FDescriptorAllocator GDescriptorAllocator[];
inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor( ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32 Count = 1 )
{
	return GDescriptorAllocator[Type].Allocate( pDevice, Count );
}
#endif // R_WITH_D3D12