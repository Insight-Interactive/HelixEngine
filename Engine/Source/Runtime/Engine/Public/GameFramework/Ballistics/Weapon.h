#pragma once

enum EWeaponType
{
	WT_Invalid,
	WT_BulletWeapon,
	WT_MeleeWeapon,
};

class FWeapon
{
public:
	FWeapon( float Damage, const char* Name = nullptr )
		: m_Damage( 1.f )
	{
		if (Name)
			strcpy_s( m_Name, Name );
		else
			ZeroMemory( m_Name, sizeof( m_Name ) );
	}
	virtual ~FWeapon()
	{
	}

	virtual EWeaponType GetStaticType() { return WT_Invalid; }
	virtual void OnFireInputPressed( const FVector3& Direction )	{}
	virtual void OnFireInputHeld( const FVector3& Direction )		{}
	virtual void OnFireInputReleased( const FVector3& Direction )	{}
	virtual bool Reload()				{ return false; }

	FTransform m_Transform;
	float m_Damage;
	char m_Name[64];
};

class FBulletWeapon : public FWeapon
{
public:
	FBulletWeapon( uint32 MagazineSize, uint32 ReserveAmmoAmount, const char* Name = nullptr )
		: FWeapon( 1.f, Name )
		, m_MaxMagazineAmmoAmount( MagazineSize )
		, m_MaxReserveAmmoAmount( ReserveAmmoAmount )
		, m_MagazineAmmo( MagazineSize )
		, m_ReserveAmmo( ReserveAmmoAmount )
	{
	}
	virtual ~FBulletWeapon()
	{
	}

	virtual EWeaponType GetStaticType() override { return WT_BulletWeapon; }

	void DoFireBullet( const FVector3& Direction );

	virtual void Fire()
	{
		if ( IsMagazineEmpty() )
			return;

		m_MagazineAmmo--;
	}

	bool Reload()
	{
		if (m_ReserveAmmo == 0)
			return false;

		uint32 NeededAmmo = m_MaxMagazineAmmoAmount - m_MagazineAmmo;
		if (NeededAmmo == 0)
			return false;

		m_ReserveAmmo -= NeededAmmo;
		if (m_ReserveAmmo < 0)
		{
			NeededAmmo += m_ReserveAmmo;
			m_ReserveAmmo = 0;
		}

		m_MagazineAmmo += NeededAmmo;

		return true;
	}

	bool IsMagazineEmpty()
	{
		return m_MagazineAmmo == 0;
	}

	uint32 m_MaxMagazineAmmoAmount;
	uint32 m_MaxReserveAmmoAmount;

	uint32 m_MagazineAmmo;
	uint32 m_ReserveAmmo;

};

static FWeapon NullWeapon( 0.f, "NullWeapon" );


class FSemiAutomaticWeapon : public FBulletWeapon
{
public:
	FSemiAutomaticWeapon()
		: FBulletWeapon( 5, 10 )
	{
	}
	virtual ~FSemiAutomaticWeapon()
	{
	}

	virtual void OnFireInputPressed( const FVector3& Direction ) override
	{
		FBulletWeapon::Fire();
	}

	virtual void OnFireInputHeld( const FVector3& Direction ) override
	{
	}

	virtual void OnFireInputReleased( const FVector3& Direction ) override
	{
		// TODO: Rechamber fx
	}
};

class FAutomaticWeapon : public FBulletWeapon
{
public:
	FAutomaticWeapon()
		: FBulletWeapon( 10, 50 )
	{
	}
	virtual ~FAutomaticWeapon()
	{
	}

	virtual void OnFireInputPressed( const FVector3& Direction ) override
	{

	}

	virtual void OnFireInputHeld( const FVector3& Direction ) override
	{

	}

	virtual void OnFireInputReleased( const FVector3& Direction ) override
	{

	}
};
