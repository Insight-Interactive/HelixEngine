// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

class PhysicsCallbackHandler
{
public:
	enum ECollisionType
	{
		CT_Enter	= 0,
		CT_Stay		= 1,
		CT_Exit		= 2,
	};
public:
	virtual void CollisionEvent( ECollisionType Type, PhysicsCallbackHandler* pCollider ) = 0;

};
