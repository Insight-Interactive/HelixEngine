#include "EnginePCH.h"

#include "AssetRegistry/DatabaseInterface.h"


const String DatabaseInterface::s_InvalidAssetPath = "<Invalid Asset Path>";

DatabaseInterface::DatabaseInterface( const String& Name )
	: m_DatabaseName( Name )
	, m_IsDirty( false )
{
}

DatabaseInterface::~DatabaseInterface()
{

}
