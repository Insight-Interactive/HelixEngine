#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class ActorDatabase : public DatabaseInterface
{
public:
	ActorDatabase();
	virtual ~ActorDatabase();
	
	virtual void Initialize( const Char* TextureDatabaseFile ) override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;
	
};
