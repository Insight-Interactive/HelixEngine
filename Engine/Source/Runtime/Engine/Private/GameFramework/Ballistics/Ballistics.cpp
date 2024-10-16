#include "EnginePCH.h"

#include "GameFramework/Ballistics/Ballistics.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Engine.h"
#include "Graphics/ShaderRegisters.h"


Ballistics::BulletManager GBulletManager;

namespace Ballistics
{
	/*static*/ uint32 BulletManager::SBulletID = 0;

	void BulletManager::Initialize()
	{
	}

	void BulletManager::UnInitialize()
	{
	}

	void BulletManager::Update( float DeltaTime )
	{
		for (size_t i = 0; i < m_WorldBullets.size(); i++)
		{
			UpdateParabolicBullet( m_WorldBullets[i], DeltaTime );
		}
	}

	void BulletManager::RenderTracers( FCommandContext& Context)
	{
		Context.BeginDebugMarker( L"Render Bullet Tracers" );

		for (size_t i = 0; i < m_WorldBullets.size(); i++)
		{
			RenderBulletTracer( Context, m_WorldBullets[i] );
		}

		Context.EndDebugMarker();
	}

	void BulletManager::RenderBulletTracer( FCommandContext& Context, HBullet& Bullet )
	{
		Bullet.Material->Bind( Context );

		
		Bullet.m_MeshWorldCB->kWorldMat = FMatrix::CreateTranslation( Bullet.CurrentPosition ).Transpose();
		Context.SetGraphicsConstantBuffer( kMeshWorld, Bullet.m_MeshWorldCB );

		Context.SetPrimitiveTopologyType( PT_TiangleList );
		Context.BindVertexBuffer( 0, Bullet.Tracer->GetMesh().GetVertexBuffer() );
		Context.BindIndexBuffer( Bullet.Tracer->GetMesh().GetIndexBuffer() );
		Context.DrawIndexedInstanced( Bullet.Tracer->GetMesh().GetNumIndices(), 1, 0, 0, 0 );
	}

	void BulletManager::UpdateParabolicBullet(HBullet& Bullet, const float& DeltaTime)
	{
		float CurrentTime = float( SystemTime::TicksToSeconds(SystemTime::GetCurrentTick()) - Bullet.StartTime );
		float NextTime = CurrentTime + DeltaTime;
		
		FVector3 CurrentPoint = FindPointOnParabola( Bullet, CurrentTime );
		FVector3 NextPoint = FindPointOnParabola( Bullet, NextTime );

		FVector3 Direction = NextPoint - CurrentPoint;
		float Length = Direction.Length();
		Direction.Normalize();

		Bullet.Lifetime -= DeltaTime;

		if (!Physics::RayCast( CurrentPoint, Direction, Length ) && Bullet.Lifetime > 0.f)
			Bullet.CurrentPosition = NextPoint;
		else
		{
			RemoveBullet( Bullet );
		}

		/*FDebugLineRenderInfo LineInfo;
		LineInfo.Start = CurrentPoint;
		LineInfo.End = NextPoint;
		LineInfo.Color = FColor::RedOpaque;
		LineInfo.IgnoreDepth = true;
		GEngine->GetGameWorld().DrawDebugLine( LineInfo );*/
	}

	FVector3 BulletManager::FindPointOnParabola( HBullet& Bullet, float Time )
	{
		FVector3 Point = Bullet.StartPosition + (Bullet.Direction * Bullet.Speed * Time);
		FVector3 GravityVec = FVector3::Down * Bullet.Gravity * Time * Time;

		return Point + GravityVec;
	}

	void MagicBullet( FVector3 Start, FVector3 Direction )
	{
		HBullet& Result = GBulletManager.CreateAndAddBullet();
		Result.StartPosition = Start;
		Result.Direction = Direction;
		Result.StartTime = SystemTime::TicksToSeconds(SystemTime::GetCurrentTick());
		Result.Speed = 4500.f;
		Result.Gravity = 9.81f;
		Result.Lifetime	 = 5.f;

		Result.m_MeshWorldCB.Create( L"Bullet Tracer" );
		Result.Tracer = GeometryGenerator::GenerateSphere( 10, 10, 10 );
		Result.Material = FAssetDatabase::GetMaterial( "M_Bullet.hmat" );
	}
}
