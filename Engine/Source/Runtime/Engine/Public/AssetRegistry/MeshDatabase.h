#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FMeshDatabase : public FDatabaseInterface
{
public:
	FMeshDatabase();
	virtual ~FMeshDatabase();
	
	virtual void Initialize( const Char* MeshDatabaseFile ) override;
	virtual void UnInitialize() override;
	
protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
