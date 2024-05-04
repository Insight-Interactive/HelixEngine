// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonEnums.h"
#include "CommonStructs.h"
#include "DescriptorAllocator.h"


class FGeometryManager;
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
class FAnimationManager;
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
// Geometry buffer overloard.
extern FGeometryBufferManager GGeometryBufferManager;
// Texture overlord.
extern FTextureManager GTextureManager;
// Font overlord.
extern FFontManager GFontManager;
// Default texture container.
extern FTexture GDefaultTextures[];
// Heap holding all shader visible textures.
extern FDescriptorHeap GTextureHeap;
// Manager of all static mesh geometry in the world.
extern FGeometryManager GGeometryManager;
// Manager for all skeletal animations in the world.
extern FAnimationManager GAnimationManager;
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