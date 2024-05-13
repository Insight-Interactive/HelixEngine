// Copyright 2021 Insight Interactive. All Rights Reserved.

/*
	Common macros use dy the rendering framework
*/
#ifndef COMMONMACROS_H
#define COMMONMACROS_H

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
#define R_MAX_JOINTS_PER_VERTEX			4
#define R_MAX_JOINTS_PER_MODEL			96

#endif
