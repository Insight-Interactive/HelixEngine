#include "EnginePCH.h"

#include "GameFramework/Components/HRenderableComponentInterface.h"


HRenderableComponentInterface::HRenderableComponentInterface( FComponentInitArgs& InitArgs )
	: HActorComponent( InitArgs )
	, m_bIsDrawEnabled( true )
{

}

HRenderableComponentInterface::~HRenderableComponentInterface()
{

}
