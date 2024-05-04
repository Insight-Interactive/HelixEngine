// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once 

#define NOMINMAX

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "DataTypes.h"


#if HE_WINDOWS
#	include <Windows.h>
#	include <process.h>
#	include <strsafe.h>
#	include <rpc.h>
#	include <shlwapi.h>

#	if HE_WINDOWS_UNIVERSAL
#		include <winrt/Windows.UI.Popups.h>
#		include <unknwn.h>
#		include <ppltasks.h>

#		include <winrt/Windows.Devices.Input.h>
#		include <winrt/Windows.Gaming.Input.h>
#		include <winrt/Windows.System.h>
//#		include <winrt/Windows.Storage.h>
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
