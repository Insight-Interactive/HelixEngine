#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FMaterialDatabase : public FDatabaseInterface
{
public:
	FMaterialDatabase();
	virtual ~FMaterialDatabase();

	virtual void Initialize( const Char* MaterialDatabaseFile ) override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
