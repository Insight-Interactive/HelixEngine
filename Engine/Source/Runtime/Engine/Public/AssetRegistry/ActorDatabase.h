#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FActorDatabase : public FDatabaseInterface
{
public:
	FActorDatabase();
	virtual ~FActorDatabase();
	
	virtual void Initialize( const Char* TextureDatabaseFile ) override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;
	
};
