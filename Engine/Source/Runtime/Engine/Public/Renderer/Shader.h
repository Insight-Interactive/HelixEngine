#pragma once

#include "AssetRegistry/Asset.h"


class FShader : public HShaderAsset
{
public:
	enum ShaderType
	{
		ST_Vertex	= 0,
		ST_Pixel	= 1,

		ST_NumShaderTypes,
	};
public:
	inline ShaderType GetType() const { return m_Type; }


protected:
	FShader( const FGUID& Guid, const char* Filepath, const ShaderType& Type )
		: HShaderAsset( Guid )
		, m_Type( Type )
	{
	}
	virtual ~FShader() {}

protected:
	ShaderType m_Type;

};

class FVertexShader : public FShader
{
public:
	FVertexShader( const FGUID& Guid, const char* Filepath)
		: FShader(Guid, Filepath, ST_Vertex)
	{
	}
	virtual ~FVertexShader() {}

};

class FPixelShader : public FShader
{
public:
	FPixelShader( const FGUID& Guid, const char* Filepath )
		: FShader( Guid, Filepath, ST_Pixel )
	{
	}
	virtual ~FPixelShader() {}

};
