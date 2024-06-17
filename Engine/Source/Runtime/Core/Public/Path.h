// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

class FPath
{
public:
	FPath();
	FPath( const char* Path );
	~FPath();

	FPath( FPath& other );

	bool IsValid() const;
	const char* GetFullPath() const;
	void SetPath( const char* Path );
	void Concat( const char* Path );

	char m_Path[HE_MAX_PATH];

};

//
// Inline function implementations
//

#include "Path.inl"
