#include "RendererPCH.h"

#include "ConstantBuffer.h"

CriticalSection FConstantBufferInterface::SBufferIdGuard;
ConstantBufferUID FConstantBufferInterface::SNextAvailableBufferID = 0;
