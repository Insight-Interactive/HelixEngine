// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/SerializeableInterface.h"

#include "GUID.h"


constexpr uint32 kMaxHObjectNameLength = 64;

/*
	The base class for all objects that exist in the world.
*/
class HObject : public FSerializeableInterface
{
	friend class FActorSerializer;
public:
	void SetObjectName( const char* Name );
	const char* GetObjectName() const;

	const FGUID& GetGuid() const;

protected:
	HObject( const char* Name );
	virtual ~HObject();

	void SetGuid(const FGUID& NewGuid);

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

protected:
	char m_Name[kMaxHObjectNameLength];
	FGUID m_Guid;

};


//
// Inline function implementations
//

FORCEINLINE const FGUID& HObject::GetGuid() const
{
	return m_Guid;
}

FORCEINLINE void HObject::SetObjectName( const char* Name )
{
	strcpy_s( m_Name, Name );
}

FORCEINLINE const char* HObject::GetObjectName() const
{
	return m_Name;
}

FORCEINLINE void HObject::SetGuid(const FGUID& NewGuid)
{
	HE_ASSERT( NewGuid.IsValid() );
	m_Guid = NewGuid;
}
