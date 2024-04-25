#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FScriptDatabase : public FDatabaseInterface
{
public:
	FScriptDatabase();
	virtual ~FScriptDatabase();

	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;
	virtual void Serialize( JsonUtility::WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
