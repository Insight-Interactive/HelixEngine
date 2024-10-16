#pragma once



struct HBullet
{
	uint32 ID;

	FVector3 CurrentPosition;

	FVector3 StartPosition;
	FVector3 Direction;
	double StartTime;
	float Speed;
	float Gravity;
	float Lifetime;
	
	// Rendering
	HStaticMesh Tracer;
	HMaterial Material;
	TConstantBuffer<MeshWorldCBData> m_MeshWorldCB;

	bool operator==( const HBullet& Other )
	{
		return this == &Other;
	}
};

namespace Ballistics
{
	class BulletManager
	{
	public:
		void Initialize();
		void UnInitialize();

		void Update( float DeltaTime );
		void RenderTracers( FCommandContext& Context );

		HBullet& CreateAndAddBullet() 
		{ 
			HBullet& Bullet = m_WorldBullets.emplace_back();
			Bullet.ID = SBulletID++;
			return Bullet;
		}
		void RemoveBullet( HBullet& Bullet ) 
		{ 
			auto Iter = std::find( m_WorldBullets.begin(), m_WorldBullets.end(), Bullet );
			if (Iter != m_WorldBullets.end())
			{
				HE_LOG( Log, "Bullet impact with id: %i", Iter->ID );
				m_WorldBullets.erase( Iter );
			}
		}

	protected:
		void UpdateParabolicBullet( HBullet& Bullet, const float& DeltaTime );
		FVector3 FindPointOnParabola( HBullet& Bullet, float Time );
		void RenderBulletTracer( FCommandContext& Context, HBullet& Bullet );

	protected:
		std::vector<HBullet> m_WorldBullets;

		static uint32 SBulletID;
	};

	void MagicBullet( FVector3 Start, FVector3 Direction );
}

extern Ballistics::BulletManager GBulletManager;
