#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class MaterialDatabase : public DatabaseInterface
{
public:
	MaterialDatabase();
	virtual ~MaterialDatabase();

	virtual void Initialize( const Char* MaterialDatabaseFile ) override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
