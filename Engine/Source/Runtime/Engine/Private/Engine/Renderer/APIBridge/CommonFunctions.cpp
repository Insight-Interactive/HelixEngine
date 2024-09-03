#include "EnginePCH.h"

#include "Engine/Renderer/APIBridge/CommonFunctions.h"


namespace APIBridge
{

	// ----------------------------------
	//		Format Conversions
	// ----------------------------------

	DXGI_FORMAT EFormatToDXGIFormat(const EFormat& Format)
	{
		switch (Format)
		{
		case EFormat::F_R8G8B8A8_UNorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default:
			R_LOG(Error, TEXT("Invalid format trying to convert ETextureFormat to DXGI_FORMAT!"));
			return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		}

	}

	EFormat DXGIFormatToEFormat(DXGI_FORMAT Format)
	{
		switch (Format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM: return EFormat::F_R8G8B8A8_UNorm;
		default:
			R_LOG(Error, TEXT("Invalid format trying to convert DXGI_FORMAT to ETextureFormat!"));
			return EFormat::F_R8G8B8A8_UNorm;
		}
	}



	// -------------------------------------
	//	D3D12 Command List Type Conversions
	// -------------------------------------

	D3D12_COMMAND_LIST_TYPE ECommandListTypeToD3D12CommandListType(const ECommandListType& Type)
	{
		switch (Type)
		{
		case ECommandListType::CLT_Direct: return D3D12_COMMAND_LIST_TYPE_DIRECT;
		case ECommandListType::CLT_Compute: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
		default:
			R_LOG(Error, TEXT("Failed to convert ECommandListType to D3D12_COMMAND_LIST_TYPE with specified enum value!"));
			return D3D12_COMMAND_LIST_TYPE_DIRECT;
		}
	}

	ECommandListType D3D12CommandListTypeToECommandListType(D3D12_COMMAND_LIST_TYPE Type)
	{
		switch (Type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: return ECommandListType::CLT_Direct;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: return ECommandListType::CLT_Compute;
		default:
			R_LOG(Error, TEXT("Failed to convert D3D12_COMMAND_LIST_TYPE to ECommandListType with specified enum value!"));
			return ECommandListType::CLT_Direct;
		}
	}

	HRESULT CreateSwapChain(void* NativeWindow, const DXGI_SWAP_CHAIN_DESC1* Desc, BOOL AllowTearing, IDXGIFactory6** ppInFactory, IUnknown* pDevice, IDXGISwapChain3** ppOutSwapChain)
	{
		HRESULT hr = S_OK;
		IDXGISwapChain1* pTempSwapChain = NULL;
#if HE_WINDOWS_DESKTOP
		hr = (*ppInFactory)->CreateSwapChainForHwnd(pDevice, *(HWND*)NativeWindow, Desc, NULL, NULL, &pTempSwapChain);
		ThrowIfFailedMsg(hr, "Failed to create swap chain for HWND!");
		if (AllowTearing)
		{
			hr = (*ppInFactory)->MakeWindowAssociation(SCast<HWND>(NativeWindow), 0);
			ThrowIfFailedMsg(hr, "Failed to Make FWindow Association");
		}
#elif HE_WINDOWS_UNIVERSAL
		//CoreWindow* pCoreWindow = RCast<CoreWindow*>( NativeWindow );
		//void* abi = winrt::get_abi( *pCoreWindow );
		hr = (*ppInFactory)->CreateSwapChainForCoreWindow(pDevice, RCast<::IUnknown*>(NativeWindow), Desc, NULL, &pTempSwapChain);
		ThrowIfFailedMsg(hr, "Failed to Create swap chain for CoreWindow!");
#endif

		hr = pTempSwapChain->QueryInterface(IID_PPV_ARGS(ppOutSwapChain));
		ThrowIfFailedMsg(hr, "Failed to query interface for temporary DXGI swapchain!");

		return hr;
	}
}