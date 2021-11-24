#pragma once

#include "RendererCore.h"
#include "TextureManager.h"

#include "Renderer/Common.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/ConstantBufferStructures.h"
#include "ConstantBuffer.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "GUID.h"
#include "StringHelper.h"


class FCommandContext;
class FShaderReflection;

typedef uint32 MaterialID;

enum EShadingModel
{
	SM_DefaultLit	= 0, // Is lit by lights inside the world. Material will be used in Physically-Based lighting.
	SM_Unlit		= 1, // Implement unlit light pass. Is not lit by lights inside the world.
	SM_Foliage		= 2, // Tree leaves and grass.
};

enum EMaterialDomain
{
	MD_Surface		= 0, // Default material that will be placed in the world.
};

enum EMaterialBlendMode
{
	MBM_Opaque		= 0, // Replace whatever color is currently in the render target when this material is rendered.
	// TODO: MDM_Masked		= 1, 
	// TODO: MBM_Translucent	= 2,
};

class FMaterial
{
	friend class MaterialManager;
public:
	FMaterial();
	virtual ~FMaterial();

	void Bind(FCommandContext& GfxContext);
	void LoadFromFile( const String& Filepath );

	bool IsValid();

	//
	// Material Info
	//

	const String& GetDebugName() const;
	MaterialID GetUID() const;
	EMaterialBlendMode GetBlendMode() const;
	EMaterialDomain GetDomain() const;
	EShadingModel GetShadingModel() const;
	bool GetIsTwoSided() const;
	void SetBlendMode( EMaterialBlendMode Type );
	void SetDomain(EMaterialDomain Domain);
	void SetShadingModel(EShadingModel Model);
	void SetIsTwoSided(bool IsTwoSided);

	//
	// Resource Binding
	//

	/*
		Set a texture in the shader.
		@param TextureName - The name of the texture declared inside the shader.
		@param Texture - The texture resource to bind to the shader.
	*/
	bool SetTexture(const Char* TextureName, HTextureRef Texture);

	/*
		Set a float value in the shader.
		@param VariableName - The name of the variable inside the declared constant buffer.
		@param Value - The value to set in the shader.
	*/
	bool SetFloat(const Char* VariableName, const float& Value);

protected:
	void Destroy();
	void SetMaterialID( const MaterialID& NewId );
	void SetDebugName( const String& Name );
	void BuildPipelineState();
	void ReflectShader(FRootSignature& outSignature, EShaderVisibility ShaderType, const FShaderReflection& Reflector);

private:
	MaterialID m_UID;
#if HE_DEBUG
	String m_DebugName;
#endif

	FGUID		m_PixelShaderGuid;
	FGUID		m_VertexShaderGuid;

	EMaterialBlendMode	m_BlendMode;
	EMaterialDomain		m_Domain;
	EShadingModel		m_ShadingModel;
	bool				m_IsTwoSided;

	FPipelineState m_Pipeline;
	FRootSignature m_RootSig;


	// TODO: Make these only accessible in the material instance.
	// Resource mappings
	
	// Constant buffer mappings for each shader.
	// < ConstBufferHashName, <RootParmIndex, CBDataForEachFrame> >
	std::unordered_map< StringHashValue, std::pair<uint32, std::vector<FConstantBufferInterface*>> > m_ConstBuffersMappings;
	// Texture mappings for each shader.
	// < HashName, <RootParamIndex, Texture> > 
	std::unordered_map< int32, std::pair<uint32, HTextureRef> > m_TextureMappings;
	// Variable mappings for const buffers.
	// < VarNameHash, <OwningBuffer, PtrForPerFrameData in m_ConstBufferMappings> >
	std::unordered_map< StringHashValue, std::vector<std::pair<FConstantBufferInterface*, uint8*>> > m_ConstBufferVarMappings;

private:
	static MaterialID SNextMaterialID;

};


//
// Inline function implementations
//


FORCEINLINE bool FMaterial::IsValid()
{
	return m_UID != HE_INVALID_MATERIAL_ID;
}

FORCEINLINE EMaterialBlendMode FMaterial::GetBlendMode() const
{
	return m_BlendMode;
}

FORCEINLINE EMaterialDomain FMaterial::GetDomain() const
{
	return m_Domain;
}

FORCEINLINE EShadingModel FMaterial::GetShadingModel() const
{
	return m_ShadingModel;
}

FORCEINLINE bool FMaterial::GetIsTwoSided() const
{
	return m_IsTwoSided;
}

FORCEINLINE const String& FMaterial::GetDebugName() const
{
	return m_DebugName;
}

FORCEINLINE MaterialID FMaterial::GetUID() const
{
	return m_UID;
}

FORCEINLINE void FMaterial::SetBlendMode( EMaterialBlendMode Type )
{
	m_BlendMode = Type;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetDomain(EMaterialDomain Domain)
{
	m_Domain = Domain;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetShadingModel(EShadingModel Model)
{
	m_ShadingModel = Model;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetIsTwoSided(bool IsTwoSided)
{
	m_IsTwoSided = IsTwoSided;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetMaterialID( const MaterialID& NewId )
{
	m_UID = NewId;
}

FORCEINLINE void FMaterial::SetDebugName( const String& Name )
{
#if HE_DEBUG
	m_DebugName = Name;
#endif
}

FORCEINLINE bool FMaterial::SetTexture(const Char* TextureName, HTextureRef Texture)
{
	StringHashValue NameHash = StringHash(TextureName);
	auto Iter = m_TextureMappings.find(NameHash);
	if (Iter != m_TextureMappings.end())
	{
		(*Iter).second.second = Texture;
		return true;
	}
	HE_LOG(Warning, TEXT("Trying to set texture with in material %s with invalid semantic name: %s"), CharToTChar(m_DebugName), CharToTChar(TextureName));
	return false;
}

FORCEINLINE bool FMaterial::SetFloat(const Char* VariableName, const float& Value)
{
	StringHashValue NameHash = StringHash(VariableName);
	auto& Iter = m_ConstBufferVarMappings.find(NameHash);
	if (Iter != m_ConstBufferVarMappings.end())
	{
		for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
		{
			// Copy the data to each frame buffer inside m_ConstBufferMappings.
			memcpy(Iter->second[i].second, &Value, sizeof(float));

			// Mark the buffer dirty to indicate a re-upload to the Gpu.
			Iter->second[i].first->SetDirty(true);
		}
		return true;
	}
	HE_LOG(Warning, TEXT("Trying to set a buffer constant in material %s with variable \"%s\" that does not exist!"), CharToTChar(m_DebugName), CharToTChar(VariableName));
	return false;
}
