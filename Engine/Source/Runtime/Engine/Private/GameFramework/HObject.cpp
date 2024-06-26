// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/HObject.h"


HObject::HObject( const char* Name )
{
	ZeroMemory( m_Name, sizeof( m_Name ) );
	strcpy_s( m_Name, Name );
}

HObject::~HObject()
{

}


void HObject::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HObject::m_Name ) );
	Output.String( m_Name );

	Output.Key( HE_STRINGIFY( HObject::m_Guid ) );
	Output.String( GetGuid().ToString().CStr() );
}

void HObject::Deserialize( const JsonUtility::ReadContext& Value )
{
	// Object Name
	JsonUtility::GetString( Value, HE_STRINGIFY( HObject::m_Name ), m_Name, sizeof( m_Name ));

	// Object GUID
	Char GuidStr[64];
	ZeroMemory( GuidStr, sizeof( GuidStr ) );
	JsonUtility::GetString( Value, HE_STRINGIFY( HObject::m_Guid ), GuidStr, sizeof(GuidStr));
	SetGuid( FGUID::CreateFromString( GuidStr ) );
}
