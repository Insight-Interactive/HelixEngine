#pragma once

#include "JsonUtility.h"

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> WriteContext;
typedef rapidjson::Value ReadContext;

class FSerializeableInterface
{
protected:
	virtual void Serialize( const Char* Filepath ) {}
	virtual void Serialize( WriteContext& Output ) = 0;
	virtual void Deserialize( const ReadContext& Value ) = 0;
	
};
