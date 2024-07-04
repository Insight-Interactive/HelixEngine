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
	Output.String( m_Guid.ToString().CStr() );
}

void HObject::Deserialize( const JsonUtility::ReadContext& Value )
{
	JsonUtility::GetString( Value, HE_STRINGIFY( HObject::m_Name ), m_Name, sizeof( m_Name ));

	JsonUtility::GetGuid( Value, HE_STRINGIFY( HObject::m_Guid ), m_Guid );
	HE_ASSERT( m_Guid.IsValid() );
}
