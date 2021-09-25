#pragma once

#include "JsonUtility.h"


class SerializeableInterface
{
public:
	virtual void Serialize( const Char* Filepath ) {}
	virtual void Serialize( rapidjson::Value& Value ) = 0;
	virtual void Deserialize( const rapidjson::Value& Value ) = 0;
	
};
