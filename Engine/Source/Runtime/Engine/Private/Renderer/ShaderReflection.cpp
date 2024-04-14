#include "EnginePCH.h"

#include "Renderer/ShaderReflection.h"


FShaderReflection::FShaderReflection(const uint8* pShaderData, const uint32& ShaderDataSize)
	: m_pReflector( NULL )
{
	ReflectShader_Internal(pShaderData, ShaderDataSize);
}

FShaderReflection::~FShaderReflection()
{

}

void FShaderReflection::ReflectShader_Internal(const uint8* pShaderData, const uint32& ShaderDataSize)
{
#if R_WITH_D3D12
	HRESULT hr = D3DReflect(pShaderData, ShaderDataSize, IID_PPV_ARGS(&m_pReflector));
	ThrowIfFailedMsg(hr, "Failed to reflect shader!");
	ResetHr(hr);

	hr = m_pReflector->GetDesc(&m_ShaderDesc);
	ThrowIfFailedMsg(hr, "Failed to get shader description!");
	ResetHr(hr);
#endif

}

bool FShaderReflection::GetResourceBindingDescripion(const uint32& Index, FShaderResourceDescription& outDesc) const
{
	HE_ASSERT(m_pReflector != NULL);

#if R_WITH_D3D12
	D3D12_SHADER_INPUT_BIND_DESC ResourceDesc = { };
	HRESULT hr = m_pReflector->GetResourceBindingDesc(Index, &ResourceDesc);
	if (FAILED( hr ))
		return false;
	ThrowIfFailedMsg(hr, "Failed to get shader resource description!");
	
	outDesc.Name		= ResourceDesc.Name;
	outDesc.Register	= ResourceDesc.BindPoint;
	switch (ResourceDesc.Type)
	{
	case D3D_SIT_CBUFFER:
		outDesc.Type = RT_ConstantBuffer;
		break;
	case D3D_SIT_TEXTURE:
		outDesc.Type = RT_Texture;
		break;
	case D3D_SIT_SAMPLER:
		outDesc.Type = RT_Sampler;
		break;
	default:
		outDesc.Type = RT_Undefined;
		break;
	}
	return true;
#endif
}

bool FShaderReflection::GetInputBindingDescription( const uint32 Index, FShaderInputDescription& outDesc ) const
{
	HE_ASSERT( m_pReflector != NULL );

#if R_WITH_D3D12
	D3D12_SIGNATURE_PARAMETER_DESC Desc = {};
	HRESULT hr = m_pReflector->GetInputParameterDesc( Index, &Desc );
	if (FAILED( hr ))
		return false;

	outDesc.Name = Desc.SemanticName;

	return true;
#endif
}

void FShaderReflection::GetConstantBufferByName(const Char* Name, FConstantBufferReflection& outBuffer) const
{
	HE_ASSERT(m_pReflector != NULL);

	ID3D12ShaderReflectionConstantBuffer* pConstbuffer = m_pReflector->GetConstantBufferByName(Name);
	HE_ASSERT(pConstbuffer != NULL);

	ZeroMemory(&outBuffer, sizeof(outBuffer));
	D3D12_SHADER_BUFFER_DESC BufferDesc = {};
	pConstbuffer->GetDesc(&BufferDesc);

	outBuffer.Name			= BufferDesc.Name;
	outBuffer.SizeInBytes	= BufferDesc.Size;
	outBuffer.Variables.resize(BufferDesc.Variables);
	for (uint32 i = 0; i < BufferDesc.Variables; ++i)
	{
		ID3D12ShaderReflectionVariable* pVariable = pConstbuffer->GetVariableByIndex(i);
		HE_ASSERT(pVariable != NULL);

		D3D12_SHADER_VARIABLE_DESC VarDesc = {};
		HRESULT hr = pVariable->GetDesc(&VarDesc);
		ThrowIfFailedMsg(hr, "Failed to get shader veriable!");

		outBuffer.Variables[i].Name			= VarDesc.Name;
		outBuffer.Variables[i].SizeInBytes	= VarDesc.Size;
	}
}
