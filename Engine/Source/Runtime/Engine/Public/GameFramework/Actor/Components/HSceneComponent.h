#pragma once

#include "GameFramework/Actor/Components/HActorComponent.h"

#include "Transform.h"


class HSceneComponent : public HActorComponent
{
public:
	HSceneComponent( const HName& Name );
	virtual ~HSceneComponent();



private:
	Transform m_Transform;

};