#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FMaterialDatabase : public FDatabaseInterface
{
public:
	FMaterialDatabase();
	virtual ~FMaterialDatabase();

	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void Serialize( WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
