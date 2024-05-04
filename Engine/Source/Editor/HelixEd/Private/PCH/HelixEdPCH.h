// Copyright 2024 Insight Interactive. All Rights Reserved.
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
//
// ImGui
#include "imgui.h"
#include "../Direct3D12/HelixEdImGuiImplD3D12.h"
#include "backends/imgui_impl_win32.h"
// Rapid Json
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filewritestream.h>


// Helix
#include "MathCore.h"
#include "Engine/EngineMacros.h"
#include "Engine/EngineDefines.h"
#include "StringHelper.h"


// System
//
// Win32 API
#if defined HE_WINDOWS
#	include <Windows.h>
#	include <strsafe.h>
#	include <wrl/client.h>	// Com library
#	include <ShlObj.h>		// File dialogs
#	include <rpc.h>

#	if R_WITH_D3D12
#		include <d3d12.h>
#		ifdef NTDDI_WIN10_RS2
#			include <dxgi1_6.h>
#		else
#			include <dxgi1_5.h>
#		endif
#endif

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


// Helix
// Files that rely on the system API.
//
#include "CoreFwd.h"
#include "PhysicsFwd.h"
#include "ScriptingFwd.h"
#include "Hash.h"
#include "Containers/TDynamicArray.h"
#include "MathCore.h"
