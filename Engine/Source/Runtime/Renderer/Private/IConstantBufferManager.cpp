#include "RendererPCH.h"

#include "IConstantBufferManager.h"


CriticalSection IConstantBufferManager::SBufferIdGuard;
ConstantBufferUID IConstantBufferManager::SNextAvailableBufferID = 0;
