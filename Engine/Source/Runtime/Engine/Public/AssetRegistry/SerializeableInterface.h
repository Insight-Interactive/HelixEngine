// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "JsonUtility.h"


class FSerializeableInterface
{
protected:
	virtual void Serialize( const Char* Filepath ) {}
	virtual void Serialize( JsonUtility::WriteContext& Output ) = 0;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) = 0;
	
};
