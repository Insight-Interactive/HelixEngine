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

#if HE_WINDOWS
#	include <Windows.h>
#	include <process.h>
#	include <strsafe.h>

#endif // HE_WINDOWS

#if HE_WINDOWS_UNIVERSAL
#	include <winrt/Windows.Foundation.h>
#	include <winrt/Windows.UI.Popups.h>
#	include <winrt/Windows.UI.Core.h>
#	include <unknwn.h>
#	include <ppltasks.h>
#endif // HE_WINDOWS_UNIVERSAL
