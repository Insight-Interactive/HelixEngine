#pragma once


#include <map>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <functional>
#include <xmmintrin.h>

// Third Party
#include "imgui.h"
#include "../Direct3D12/HelixEdImGuiImplD3D12.h"
#include "backends/imgui_impl_win32.h"


#include "MathCore.h"
#include "Engine/EngineMacros.h"
#include "Engine/EngineDefines.h"



#if defined HE_WINDOWS
#	include <Windows.h>
#	include <strsafe.h>
#	include <d3d12.h>
#	include <wrl/client.h>

//	Input
// 
#	if HE_INPUT_USE_XINPUT
#	   include <Xinput.h>
#	   pragma comment(lib, "xinput9_1_0.lib")

#	   define DIRECTINPUT_VERSION 0x0800
#	   include <dinput.h>
#	   pragma comment(lib, "dinput8.lib")
#	   pragma comment(lib, "dxguid.lib")

#	endif // HE_INPUT_USE_XINPUT

#endif // HE_WINDOWS

#include "CoreFwd.h"
#include "Hash.h"
#include "Containers/TDynamicArray.h"
