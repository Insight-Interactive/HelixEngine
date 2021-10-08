#include "RendererPCH.h"

#include "IConstantBufferManager.h"


CriticalSection FConstantBufferManager::SBufferIdGuard;
ConstantBufferUID FConstantBufferManager::SNextAvailableBufferID = 0;
