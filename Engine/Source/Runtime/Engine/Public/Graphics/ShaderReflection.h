// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

const int32 kInvalidShaderResourceCount = -1;
const int32 kInvalidTextureNormalCount = -1;

enum EShaderResourceType
{
	RT_Undefined		= -1,
	RT_ConstantBuffer	=  0,
	RT_Texture			=  1,
	RT_Sampler			=  2,
};

/*
	Represents a resource bound to a shader stage.
*/
struct FShaderResourceDescription
{
	const Char*			Name;
	uint32				Register;
	EShaderResourceType Type;
};

struct FShaderInputDescription
{
	const Char* Name;
};

/*
	Represents a shader variable bound to a shader stage.
*/
struct FShaderVariableDescription
{
	const Char* Name;
	uint32		SizeInBytes;
};

/*
	Represents a constant buffer bound to a shader stage.
*/
struct FConstantBufferReflection
{
	const Char*								Name;
	uint32									SizeInBytes;
	std::vector<FShaderVariableDescription> Variables;
};

class FShader;

/*
	Reflects a shader and gives runtime information about it.
*/
class FShaderReflection
{
public:
	FShaderReflection(const FShader& Shader);
	FShaderReflection(const uint8* pShaderData, const uint32& ShaderDataSize);
	~FShaderReflection();

	/*
		Returns the number of resources bound to the specified shader.
	*/
	uint32 GetNumBoundResources() const;
	
	/*
		Returns the number of texture use instructions in the specified shader.
	*/
	uint32 GetNumTextureNormalInstructions() const;

	/*
		Retrieves more information about a resource bound to the specified shader.
	*/
	void GetResourceBindingDescripion(const uint32& Index, FShaderResourceDescription& outDesc) const;

	/*
	Returns true if succeeded, false if not.
	*/
	bool GetInputBindingDescription( const uint32 Index, FShaderInputDescription& outDesc ) const;

	/*
		Retreives more information about a constant buffer bound to the specified shader.
	*/
	void GetConstantBufferByName(const Char* Name, FConstantBufferReflection& outBuffer) const;

private:
	/*
		Internal function that relects the shader.
	*/
	void ReflectShader_Internal(const uint8* pShaderData, const uint32& ShaderDataSize);

protected:
#if R_WITH_D3D12
	ID3D12ShaderReflection* m_pReflector;
	D3D12_SHADER_DESC m_ShaderDesc;

#endif
};


//
// Inline function implementations
//

FORCEINLINE uint32 FShaderReflection::GetNumBoundResources() const
{
#if R_WITH_D3D12
	return m_ShaderDesc.BoundResources;
#else
	return kInvalidShaderResourceCount;
#endif
}

FORCEINLINE uint32 FShaderReflection::GetNumTextureNormalInstructions() const
{
#if R_WITH_D3D12
	return m_ShaderDesc.TextureNormalInstructions;
#else
	return kInvalidTextureNormalCount;
#endif
}
