#pragma once

#include "RendererFwd.h"
#include "CommonEnums.h"


namespace APIBridge
{
	// ----------------------------------
	//		Format Conversions
	// ----------------------------------

	DXGI_FORMAT EFormatToDXGIFormat(const EFormat& Format);

	EFormat DXGIFormatToEFormat(DXGI_FORMAT Format);



	// -------------------------------------
	//	D3D12 Command List Type Conversions
	// -------------------------------------

	D3D12_COMMAND_LIST_TYPE ECommandListTypeToD3D12CommandListType(const ECommandListType& Type);

	ECommandListType D3D12CommandListTypeToECommandListType(D3D12_COMMAND_LIST_TYPE Type);

	HRESULT CreateSwapChain(void* NativeWindow, const DXGI_SWAP_CHAIN_DESC1* Desc, BOOL AllowTearing, IDXGIFactory6** ppInFactory, IUnknown* pDevice, IDXGISwapChain3** ppOutSwapChain);

}