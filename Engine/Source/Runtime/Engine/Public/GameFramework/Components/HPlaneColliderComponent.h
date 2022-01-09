#pragma once

#include "GameFramework/Components/HColliderComponentInterface.h"


HCOMPONENT()
class HPlaneColliderComponent : public HColliderComponentInterface
{
	friend class AActor;
	using Super = HColliderComponentInterface;
public:
	HE_COMPONENT_GENERATED_BODY( HPlaneColliderComponent );

protected:
	virtual void OnCreate() override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

private:
	float m_Width;
	float m_Height;
};
