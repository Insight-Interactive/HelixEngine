// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FMaterialDatabase : public FDatabaseInterface
{
public:
	FMaterialDatabase();
	virtual ~FMaterialDatabase();

	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;
	virtual void Serialize( JsonUtility::WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
