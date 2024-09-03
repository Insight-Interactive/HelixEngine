#include "EnginePCH.h"

#include "Engine/Renderer/Shader.h"
#include "FileSystem.h"


FShader::FShader()
{

}

FShader::~FShader()
{

}

void FShader::LoadFromFile( const FPath& Path )
{
	if (m_ShaderData.GetDataSize() > 0)
		m_ShaderData.Invalidate();

	m_ShaderData = FileSystem::ReadRawData( Path.GetFullPath() );
}
