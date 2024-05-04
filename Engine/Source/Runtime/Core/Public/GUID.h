// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "DebugAssert.h"
#include "DataTypes.h"

#if HE_WINDOWS
#	include <Rpc.h>
#endif

class FGUID;

class GUIDString
{
public:
	GUIDString( const FGUID& Guid );
	~GUIDString();

	const char* CStr();

private:
	unsigned char* m_GuidString;

};

class FGUID
{
public:
	FGUID();
	FGUID(const Char* GuidStr);
	FGUID(uint32 InitialValue);
	FGUID(const FGUID& Other);
	~FGUID();
	FGUID& operator = (const FGUID& Other);
	bool operator == (const FGUID& Other) const;
	bool operator != (const FGUID& Other) const;

	bool IsValid() const;
	const void* GetNativeGUID() const;
	GUIDString ToString() const;

	static FGUID Generate();
	static void Generate( FGUID& outResult );
	static FGUID CreateFromString( const Char* String );
	static void CreateFromString( const Char* String, FGUID& outResult );
	
public:
	static const FGUID Invalid;
	static const FGUID Zero;

private:
#if HE_WINDOWS
	UUID m_UUID;
#endif


};

class FGUIDHashFunction
{
public:
	size_t operator () ( const FGUID& Guid ) const noexcept(true)
	{
		std::string str = Guid.ToString().CStr();
		return std::hash<std::string>{}(str);
	}
};


//
// Inline function implementations
//

// GUIDString
//

FORCEINLINE GUIDString::GUIDString( const FGUID& Guid )
	: m_GuidString( NULL )
{
#if HE_WINDOWS
	RPC_STATUS rpcStat = UuidToStringA( (const UUID*)Guid.GetNativeGUID(), &m_GuidString );
	HE_ASSERT( rpcStat == RPC_S_OK );
#endif
}

FORCEINLINE GUIDString::~GUIDString()
{
	if (m_GuidString)
	{
#if HE_WINDOWS
		RPC_STATUS rpcStat = RpcStringFreeA( &m_GuidString );
		HE_ASSERT( rpcStat == RPC_S_OK );
#endif
	}
}

FORCEINLINE const char* GUIDString::CStr()
{
	return (const char*)m_GuidString;
}


// FGUID
//

FORCEINLINE FGUID::FGUID()
{
#if HE_WINDOWS
	ZeroMemory(&m_UUID, sizeof(m_UUID));
#endif
}

FORCEINLINE FGUID::FGUID(const Char* GuidStr)
	: FGUID()
{
	FGUID::CreateFromString(GuidStr, *this);
}

FORCEINLINE FGUID::FGUID(uint32 InitialValue)
{
#if HE_WINDOWS
	memset(&m_UUID, InitialValue, sizeof(m_UUID));
#endif
}

FORCEINLINE FGUID::FGUID( const FGUID& Other )
{
	*this = Other;
}

FORCEINLINE FGUID::~FGUID()
{
}

FORCEINLINE bool FGUID::IsValid() const
{
	return *this != FGUID::Invalid;
}

FORCEINLINE FGUID& FGUID::operator = (const FGUID& Other)
{
	if (this == &Other)
		return *this;

	memcpy(&this->m_UUID, &Other.m_UUID, sizeof(m_UUID));

	return *this;
}

FORCEINLINE bool FGUID::operator == (const FGUID& Other) const
{
#if HE_WINDOWS
	return memcmp(&m_UUID, &Other.m_UUID, sizeof(m_UUID)) == 0;
#endif
}

FORCEINLINE bool FGUID::operator != (const FGUID& Other) const
{
	return !(*this == Other);
}

FORCEINLINE /*static*/ FGUID FGUID::Generate()
{
	FGUID Result;
	FGUID::Generate( Result );
	return Result;
}

FORCEINLINE /*static*/ void FGUID::Generate( FGUID& outResult )
{
#if HE_WINDOWS
	RPC_STATUS rpsStat = UuidCreate( &outResult.m_UUID );
	HE_ASSERT( rpsStat == RPC_S_OK );
#endif
}

FORCEINLINE /*static*/ FGUID FGUID::CreateFromString( const Char* String )
{
	HE_ASSERT( String != NULL ); // 
	FGUID Result;
	CreateFromString( String, Result );
	return Result;
}

FORCEINLINE /*static*/ void FGUID::CreateFromString( const Char* String, FGUID& outResult )
{
#if HE_WINDOWS
	RPC_STATUS rpsStat = UuidFromStringA( (RPC_CSTR)String, &outResult.m_UUID );
	HE_ASSERT( rpsStat == RPC_S_OK );
#endif
}

FORCEINLINE const void* FGUID::GetNativeGUID() const
{
#if HE_WINDOWS
	return (const void*)&m_UUID;
#endif
}

FORCEINLINE GUIDString FGUID::ToString() const
{
	return GUIDString( *this );
}