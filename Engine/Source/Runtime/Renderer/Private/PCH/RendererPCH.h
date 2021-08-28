#pragma once


#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <string>
#include <unordered_map>
#include <sstream>
#include <intrin0.h>

#include "RendererFwd.h"

#if HE_WINDOWS
#	include <wrl/client.h>
#endif

#if R_WITH_D3D
#	include <d3d12.h>
#	include <d2d1_3.h>
#	include <dwrite.h>
#	include <d3d11on12.h>
#	include "../Direct3D12/d3dx12.h"

#	ifdef NTDDI_WIN10_RS2
#		include <dxgi1_6.h>
#	else
#		include <dxgi1_5.h>
#	endif

#	if R_TRACK_RENDER_EVENTS
#		include "WinPixEventRuntime/pix3.h"
#	endif

#endif

#include "MathCore.h"
