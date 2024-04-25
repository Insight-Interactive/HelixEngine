#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FTextureDatabase : public FDatabaseInterface
{
public:
	FTextureDatabase();
	virtual ~FTextureDatabase();
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;
	virtual void Serialize( JsonUtility::WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
