#include "EnginePCH.h"

#include "GameFramework/Ballistics/Weapon.h"

#include "Engine/Engine.h"
#include "Engine/Physics/Physics.h"

void FBulletWeapon::DoFireBullet( const FVector3& Direction )
{
	HWorld& World = GEngine->GetGameWorld();
	FVector3 BulletFirePos = m_Transform.GetPosition();
	FVector3 HitPos;
	FRaycastHitInfo HitInfo = {};
	float MaxTraceDistance = 10000.f; // TODO: Should be the range of the weapon

	World.Raycast( BulletFirePos, Direction, MaxTraceDistance, &HitInfo);

	if (HitInfo.AnyHit)
		HitPos = HitInfo.HitPos;
	else
		HitPos = BulletFirePos + Direction * MaxTraceDistance;
}