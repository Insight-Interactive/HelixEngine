#pragma once


class FDamageable
{
public:

	bool IsAlive()
	{
		return m_Health <= 0.f;
	}

	void SetHealth( float Health )
	{
		m_Health = Health;
	}

	float GetHealth()
	{
		return m_Health;
	}

	float DoDamage( float Amount )
	{
		m_Health -= Amount;
		Math::Min( m_Health, 0.f );
	}

private:
	void OnDeath() {}

private:
	float m_Health;
};
