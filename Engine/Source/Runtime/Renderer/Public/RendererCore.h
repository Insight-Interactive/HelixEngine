// Copyright Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonEnums.h"
#include "CommonStructs.h"


#define HE_SIMULTANEOUS_RENDER_TARGET_COUNT		( 8 )
#define HE_DEFAULT_STENCIL_READ_MASK			( 0xFF )
#define HE_DEFAULT_STENCIL_WRITE_MASK			( 0xFF )
#define HE_DEFAULT_DEPTH_BIAS					( 0 )
#define HE_DEFAULT_DEPTH_BIAS_CLAMP				( 0.f )
#define HE_DEFAULT_SLOPE_SCALED_DEPTH_BIAS		( 0.f )
#define HE_INVALID_VERTEX_BUFFER_HANDLE			( VertexBufferUID )( -1 )
#define HE_INVALID_INDEX_BUFFER_HANDLE			( IndexBufferUID )( -1 )
#define	HE_APPEND_ALIGNED_ELEMENT				( 0xFFFFFFFF )
#define HE_MAX_CONSTANT_BUFFER_SIZE				( 256 )
#define HE_INVALID_CONSTANT_BUFFER_HANDLE		( ConstantBufferUID )( -1 )
#define HE_INVALID_GPU_ADDRESS					( -1 )
#define HE_INVALID_MATERIAL_ID					( -1 )
#define HE_DESCRIPTOR_RANGE_OFFSET_APPEND		( 0xFFFFFFFF )

#define HE_D3D12_RENDER_BACKEND_NAME "Direct3D 12"
#define HE_D3D11_RENDER_BACKEND_NAME "Direct3D 11"


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
class HTexture;
class HTextureRef;
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
class FConstantBuffer;
class FDescriptorHandle;
// Structs
struct FPipelineStateDesc;
struct FRootSignatureDesc;
// Enums
enum EResourceHeapType;


// ------------------
//	Extern Variables
// ------------------
// The render context manages the lifetime of these objects.
// 
// Command context managment overlord.
extern FCommandManager* GCommandManager;
// Graphics context management overlord.
extern FContextManager* GContextManager;
// Graphics rendering device.
extern FRenderDevice* GDevice;
// Geometry buffer overloard.
extern FGeometryBufferManager* GGeometryManager;
// Constant buffer overlord.
extern FConstantBufferManager* GConstantBufferManager;
// Texture overlord.
extern FTextureManager* GTextureManager;
// Default texture containter.
// The texture manager manages the lifetime of the objects this array's elements point too.
// Each API has their own texture type.
extern HTexture* GDefaultTextures[];
// Heap holding all shader visible textures.
extern FDescriptorHeap* GTextureHeap;
// Manager of all static mesh geometry in the world.
extern FStaticGeometryManager GStaticGeometryManager;

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
	Constructs a core render component and returns the result. Used during initialization of
	core rendering components such as the swapchain, render device, managers etc.
	Example Usage: D3D12Device pNewDevice = CreateRenderComponentObject<D3D12Device>(pDevice); Where pDevice is of type FRenderDevice.
	@param ppBase - The interface to store the result into.
	@param args - Optional additional arguments for the object's constructor.
*/
template <typename DerivedType, typename BaseType, typename ... InitArgs>
inline DerivedType* CreateRenderComponentObject(BaseType** ppBase, InitArgs ... args)
{
	(*ppBase) = new DerivedType(args...);
	DerivedType* pDerivedClass = DCast<DerivedType*>((*ppBase));
	HE_ASSERT(pDerivedClass != NULL);

	return pDerivedClass;
}

/*
	Returns a default texture given an enum value.
*/
HTexture* GetDefaultTexture(EDefaultTexture TexID);
