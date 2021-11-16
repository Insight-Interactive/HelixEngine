#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FMeshDatabase : public FDatabaseInterface
{
public:
	FMeshDatabase();
	virtual ~FMeshDatabase();

	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void Serialize( WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;
	
protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
