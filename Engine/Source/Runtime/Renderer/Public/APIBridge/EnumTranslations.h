#pragma once

#include "CoreFwd.h"


const uint32 API_COMMAND_LIST_TYPE_DIRECT =
#if HE_WINDOWS
D3D12_COMMAND_LIST_TYPE_DIRECT;
#else
0;
#pragma message("Warning: Current platform does not override command list type: direct.")
#endif

const uint32 API_COMMAND_LIST_TYPE_COMPUTE =
#if HE_WINDOWS
	D3D12_COMMAND_LIST_TYPE_COMPUTE;
#else
1;
#pragma message("Warning: Current platform does not override command list type: compute.")
#endif

const uint32 API_COMMAND_LIST_TYPE_COPY =
#if HE_WINDOWS
	D3D12_COMMAND_LIST_TYPE_COPY;
#else
2;
#pragma message("Warning: Current platform does not override command list type: copy.")
#endif

const uint32 API_COMMAND_LIST_TYPE_BUNDLE =
#if HE_WINDOWS
	D3D12_COMMAND_LIST_TYPE_BUNDLE;
#else
3;
#pragma message("Warning: Current platform does not override command list type: bundle.")
#endif