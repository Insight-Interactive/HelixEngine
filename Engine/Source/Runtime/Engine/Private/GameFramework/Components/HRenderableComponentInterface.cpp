#include "EnginePCH.h"

#include "GameFramework/Components/HRenderableComponentInterface.h"


HRenderableComponenetInterface::HRenderableComponenetInterface( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
	, m_bIsDrawEnabled( true )
{

}

HRenderableComponenetInterface::~HRenderableComponenetInterface()
{

}

void HRenderableComponenetInterface::OnCreate()
{

}

void HRenderableComponenetInterface::OnDestroy()
{

}

void HRenderableComponenetInterface::Render( FCommandContext& GfxContext )
{

}
