#pragma once

/*
	The base class for all objects that exist in the world.
*/
class HObject
{
public:
	void SetObjectName( const HName& Name );
	const HName& GetObjectName() const;

protected:
	HObject( const HName& Name );
	virtual ~HObject();

private:
	HName m_Name;

};


//
// Inline function implementations
//

inline void HObject::SetObjectName( const HName& Name )
{
	m_Name = Name;
}

inline const HName& HObject::GetObjectName() const
{
	return m_Name;
}