#include "RendererPCH.h"

#include "CommandContext.h"
#include "RendererCore.h"



FCommandContext& FCommandContext::Begin(const TChar* ID)
{
	FCommandContext* NewContext = GContextManager.AllocateContext(ECommandListType::CLT_Direct);
	NewContext->SetID(ID);
	NewContext->BeginDebugMarker(ID);

	return *NewContext;
}