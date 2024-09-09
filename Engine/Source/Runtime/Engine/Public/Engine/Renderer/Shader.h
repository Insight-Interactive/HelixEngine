// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "Path.h"
#include "DataTypeWrappers.h"


class FShader
{
public:
	FShader();
	~FShader();
	
	void LoadFromFile( const FPath& Path );
	const uint8* GetData() const;
	const uint32 GetDataSize() const;

private:
	DataBlob m_ShaderData;

};


// Inline function implementations
//

inline const uint8* FShader::GetData() const
{
	return m_ShaderData.GetBufferPointer();
}

inline const uint32 FShader::GetDataSize() const
{
	return m_ShaderData.GetDataSize();
}
