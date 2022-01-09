// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PhysicsPCH.h"

#include "RigidBody.h"


RigidBody::RigidBody()
	: m_IsStatic( false )
	, m_pRigidActor( nullptr )
	, m_pPhysicsMaterial( nullptr )

{
}

RigidBody::~RigidBody()
{
}
