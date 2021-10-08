#include "RendererPCH.h"

#include "ICommandContext.h"
#include "RendererCore.h"


FContextManager::FContextManager()
{
}

FContextManager::~FContextManager()
{
	for (size_t i = 0; i < cx_ContextPoolSize; ++i)
	{
		for (size_t j = 0; j < m_ContextPool[i].size(); ++j)
		{
			delete m_ContextPool[i][j];
		}
	}
}

FCommandContext& FCommandContext::Begin(const TChar* ID)
{
	FCommandContext* NewContext = GContextManager->AllocateContext(ECommandListType::CLT_Direct);
	NewContext->SetID(ID);
	NewContext->BeginDebugMarker(ID);

	return *NewContext;
}