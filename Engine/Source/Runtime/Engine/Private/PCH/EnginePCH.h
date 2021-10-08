// Copyright 2021 Insight Interactive. All Rights Reserved.
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
#include <iostream>
#include <fstream>

#include "Engine/EngineMacros.h"
#include "Engine/EngineDefines.h"

#if defined HE_WINDOWS
#	include <Windows.h>
#	include <shellapi.h>
#	include <hidusage.h>
#	include <strsafe.h>
#	include <d3d12.h>
#	include <ShlObj.h>		// File dialogs
#	include <Shlwapi.h>		// IUnknown overrides
#	include <Windowsx.h>

//	Input
// 
#	if HE_INPUT_USE_XINPUT
#		include <Xinput.h>
#		pragma comment(lib, "xinput9_1_0.lib")
#		pragma comment(lib, "Xinput.lib")

#	endif // HE_INPUT_USE_XINPUT

#	if HE_WINDOWS_UNIVERSAL
#		include <winrt/Windows.Devices.Input.h>
#		include <winrt/Windows.Gaming.Input.h>
#		include <winrt/Windows.System.h>
//		#include <winrt/Windows.Storage.h>
#		include <winrt/Windows.UI.Core.h>
#		include <winrt/Windows.UI.Input.h>
#		include <winrt/Windows.Foundation.h>
//#		include <winrt/Windows.Storage.Pickers.h>
#		include <winrt/Windows.ApplicationModel.h>
#		include <winrt/Windows.Graphics.Display.h>
#		include <winrt/Windows.UI.ViewManagement.h>
#		include <winrt/Windows.ApplicationModel.Core.h>
#		include <winrt/Windows.ApplicationModel.Activation.h>

		using namespace winrt::Windows::UI::Core;
		using namespace winrt::Windows::ApplicationModel;
		using namespace winrt::Windows::ApplicationModel::Core;
		using namespace winrt::Windows::ApplicationModel::Activation;
		using namespace winrt::Windows::UI::Core;
		using namespace winrt::Windows::UI::Input;
		using namespace winrt::Windows::UI::Popups;
		using namespace winrt::Windows::UI::ViewManagement;
		using namespace winrt::Windows::System;
		using namespace winrt::Windows::Foundation;
		using namespace winrt::Windows::Graphics::Display;

#	endif // HE_WINDOWS_UNIVERSAL

#endif // HE_WINDOWS

#include "CoreFwd.h"
#include "MathCore.h"

#include "Hash.h"
#include "Containers/TDynamicArray.h"

// Third Party
//
// Rapid Json
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filewritestream.h>
