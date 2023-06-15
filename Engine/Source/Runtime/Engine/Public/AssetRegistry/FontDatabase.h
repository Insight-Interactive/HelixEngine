#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FFontDatabase : public FDatabaseInterface
{
public:
	FFontDatabase();
	virtual ~FFontDatabase();

	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void Serialize( WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
