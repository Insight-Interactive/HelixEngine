#pragma once

#include "AssetRegistry/DatabaseInterface.h"


class FAnimationDatabase : public FDatabaseInterface
{
public:
	FAnimationDatabase();
	virtual ~FAnimationDatabase();

	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void Serialize( WriteContext& Writer ) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;
	
protected:
	virtual void SerializeToFile_Implementation( const Char* Filepath ) override;

};
