// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/DatabaseInterface.h"


const String FDatabaseInterface::SInvalidAssetPath = "<Invalid Asset Path>";

FDatabaseInterface::FDatabaseInterface( const String& Name )
	: m_DatabaseName( Name )
	, m_IsDirty( false )
{
}

FDatabaseInterface::~FDatabaseInterface()
{

}
