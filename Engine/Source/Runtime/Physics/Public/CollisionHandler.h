#pragma once

class PhysicsCallbackHandler
{
public:
	enum ECollisionType
	{
		CT_Enter,
		CT_Stay,
		CT_Exit,
	};
public:
	virtual void CollisionEvent( ECollisionType Type, PhysicsCallbackHandler* pCollider ) = 0;

};
