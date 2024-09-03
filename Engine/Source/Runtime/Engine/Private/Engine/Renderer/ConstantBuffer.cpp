#include "EnginePCH.h"

#include "Engine/Renderer/ConstantBuffer.h"

CriticalSection FConstantBufferInterface::SBufferIdGuard;
ConstantBufferUID FConstantBufferInterface::SNextAvailableBufferID = 0;
