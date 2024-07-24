#pragma once


class FWeapon
{
public:

	void Shoot()
	{
		if (IsMagazineEmpty())
			return;

		m_MagazineAmmo--;
	}

	bool Reload()
	{
		if (m_ReserveAmmo == 0)
			return false;

		float NeededAmmo = m_MaxMagazineAmmoAmount - m_MagazineAmmo;
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

//private:
	float m_MaxMagazineAmmoAmount;
	float m_MaxReserveAmmoAmount;

	float m_MagazineAmmo;
	float m_ReserveAmmo;
	
	float m_Damage;

	char m_Name[64];
};
