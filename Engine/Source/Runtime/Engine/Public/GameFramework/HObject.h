// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GUID.h"

/*
	The base class for all objects that exist in the world.
*/
class HObject
{
public:
	void SetObjectName( const HName& Name );
	const HName& GetObjectName() const;

	const FGUID& GetGuid() const;

protected:
	HObject( const HName& Name = TEXT("Unamed HObject") );
	virtual ~HObject();

	void SetGuid(const FGUID& NewGuid);

private:
	HName m_Name;
	FGUID m_Guid;

};


//
// Inline function implementations
//

FORCEINLINE const FGUID& HObject::GetGuid() const
{
	return m_Guid;
}

FORCEINLINE void HObject::SetObjectName( const HName& Name )
{
	m_Name = Name;
}

FORCEINLINE const HName& HObject::GetObjectName() const
{
	return m_Name;
}

FORCEINLINE void HObject::SetGuid(const FGUID& NewGuid)
{
	m_Guid = NewGuid;
}
