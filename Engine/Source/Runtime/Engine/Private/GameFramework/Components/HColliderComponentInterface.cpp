#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponentInterface.h"
#include "..\..\..\Public\GameFramework\Components\HColliderComponentInterface.h"


HColliderComponentInterface::HColliderComponentInterface( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
{

}

HColliderComponentInterface::~HColliderComponentInterface()
{

}


void HColliderComponentInterface::Serialize( WriteContext& Output )
{

}

void HColliderComponentInterface::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

	JsonUtility::GetBoolean( Value[1], HE_STRINGIFY( m_DefaultDontUse ), m_DefaultDontUse );
}

void HColliderComponentInterface::OnCreate()
{
	Super::OnCreate(); 
}
