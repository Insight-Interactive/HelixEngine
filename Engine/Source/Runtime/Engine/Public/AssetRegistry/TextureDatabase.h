#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FTextureDatabase : public FDatabaseInterface
{
public:
	FTextureDatabase();
	virtual ~FTextureDatabase();

	virtual void Initialize( const Char* TextureDatabaseFile ) override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
