// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/ManagedAsset.h"

#include "Graphics/Common.h"
#include "AssetRegistry/SerializeableInterface.h"
#include "Engine/Renderer/RootSignature.h"
#include "Engine/Renderer/PipelineState.h"
#include "Engine/Renderer/TextureManager.h"
#include "Engine/Renderer/ConstantBuffer.h"


class FCommandContext;
class FShaderReflection;

enum EShadingModel
{
	SM_DefaultLit	= 0, // Is lit by lights inside the world. Material will be used in Physically-Based lighting.
	SM_Unlit		= 1, // Implement unlit light pass. Is not lit by lights inside the world.
	SM_Foliage		= 2, // Tree leaves and grass.
};

enum EMaterialDomain
{
	MD_Surface = 0, // Default material that will be placed in the world.
};

enum EMaterialBlendMode
{
	MBM_Opaque = 0, // Replace whatever color is currently in the render target when this material is rendered.
	// TODO: MDM_Masked		= 1, 
	// TODO: MBM_Translucent	= 2,
};


// < VarNameHash, <OwningBuffer, <PtrForPerFrameData in m_ConstBufferMappings(<Buffer Pointer, DataPointer for variable inside the const buffer>) >> >
typedef std::unordered_map< StringHashValue, std::vector< std::pair<FConstantBufferInterface*, uint8* > > >		ConstantBufferVariableMap;
// < ConstBufferHashName, <RootParmIndex, CBDataForEachFrame> >
typedef std::unordered_map< StringHashValue, std::pair< uint32, std::vector< FConstantBufferInterface* > > >	ConstantBufferMap;
// < HashName, <RootParamIndex, Texture> > 
typedef std::unordered_map< int32, std::pair< uint32, HTexture > >											TextureMap;

class FMaterial : public ManagedAsset<FMaterial>
{
	friend class MaterialManager;
	friend class FMaterialInstance;
public:
	FMaterial();
	virtual ~FMaterial();

	void Bind( FCommandContext& GfxContext );
	virtual void LoadFromFile( const String& Filepath );
	void Compile();
	bool IsValid();

	//
	// Material Info
	//

	const String& GetDebugName() const;
	FGUID GetGuid() const;
	EMaterialBlendMode GetBlendMode() const;
	EMaterialDomain GetDomain() const;
	EShadingModel GetShadingModel() const;
	EFillMode GetFillMode() const;
	bool GetIsTwoSided() const;
	bool GetIsWireframe() const;
	void SetBlendMode( EMaterialBlendMode Type );
	void SetDomain( EMaterialDomain Domain );
	void SetShadingModel( EShadingModel Model );
	void SetFillMode( EFillMode FillMode );
	void SetIsTwoSided( bool IsTwoSided );

	//
	// Resource Binding
	//

	/*
		Set a texture in the shader.
		@param TextureName - The name of the texture declared inside the shader.
		@param Texture - The texture resource to bind to the shader.
	*/
	bool SetTexture( const Char* TextureName, HTexture Texture );

	/*
		Set a float value in the shader.
		@param VariableName - The name of the variable inside the declared constant buffer.
		@param Value - The value to set in the shader.
	*/
	bool SetFloat( const Char* VariableName, const float& Value );

	/*
		Set a float2 value in the shader.
		@param VariableName - The name of the variable inside the declared constant buffer.
		@param Value - The value to set in the shader.
	*/
	bool SetVector2( const Char* VariableName, const FVector2& Value );

	/*
		Set a float3 value in the shader.
		@param VariableName - The name of the variable inside the declared constant buffer.
		@param Value - The value to set in the shader.
	*/
	bool SetVector3( const Char* VariableName, const FVector3& Value );

	/*
		Set a float4 value in the shader.
		@param VariableName - The name of the variable inside the declared constant buffer.
		@param Value - The value to set in the shader.
	*/
	bool SetVector4( const Char* VariableName, const FVector4& Value );

protected:
	void Destroy();
	void SetMaterialGUID( const FGUID& NewId );
	void SetDebugName( const String& Name );
	void BuildPipelineState();
	void ReflectShader( FRootSignature& outSignature, EShaderVisibility ShaderType, const FShaderReflection& Reflector );

	void BuildRootSignature();

	template <typename T>
	bool SetShaderFloatVar( const Char* VariableName, const T& Value );
	void DeserializeTextureReflection( const JsonUtility::ReadContext& TextureJson );
	void DeserializeBufferReflection( const JsonUtility::ReadContext& BufferJson );

protected:
#if HE_DEBUG
	String m_DebugName;
#endif
	FGUID m_GUID;

	FGUID		m_PixelShaderGuid;
	FGUID		m_VertexShaderGuid;

	EMaterialBlendMode	m_BlendMode;
	EFillMode			m_FillMode;
	EMaterialDomain		m_Domain;
	EShadingModel		m_ShadingModel;
	bool				m_IsTwoSided;

	FPipelineState m_Pipeline;
	FRootSignature m_RootSig;


	// Resource mappings
	// TODO: Make these only accessible in the material instance.
	//

	// Constant buffer mappings for each shader.
	ConstantBufferMap			m_ConstBuffersMappings;
	// Texture mappings for each shader.
	TextureMap					m_TextureMappings;
	// Variable mappings for const buffers.
	ConstantBufferVariableMap	m_ConstBufferVarMappings;

};


class FMaterialInstance : public FMaterial
{
	using Super = FMaterial;
public:
	FMaterialInstance();
	virtual ~FMaterialInstance();

	virtual void LoadFromFile( const String& Filepath ) override;
	void CreateFromParent( const char* ParentMaterial );


protected:
	FGUID m_InstanceGUID;

};

typedef ManagedAsset<FMaterial> ManagedMaterial;
typedef AssetRef<FMaterial> HMaterial;


//
// Inline function implementations
//


// FMaterial
//

FORCEINLINE void FMaterial::Compile()
{
	BuildPipelineState();
}

FORCEINLINE bool FMaterial::IsValid()
{
	return m_GUID.IsValid();
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

FORCEINLINE EFillMode FMaterial::GetFillMode() const
{
	return m_FillMode;
}

FORCEINLINE bool FMaterial::GetIsTwoSided() const
{
	return m_IsTwoSided;
}

FORCEINLINE bool FMaterial::GetIsWireframe() const
{
	return GetFillMode() == FM_WireFrame;
}

FORCEINLINE const String& FMaterial::GetDebugName() const
{
#if HE_DEBUG
	return m_DebugName;
#else
	return "";
#endif
}

FORCEINLINE FGUID FMaterial::GetGuid() const
{
	return m_GUID;
}

FORCEINLINE void FMaterial::SetBlendMode( EMaterialBlendMode Type )
{
	m_BlendMode = Type;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetDomain( EMaterialDomain Domain )
{
	m_Domain = Domain;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetShadingModel( EShadingModel Model )
{
	m_ShadingModel = Model;
	BuildPipelineState();
}

FORCEINLINE	void FMaterial::SetFillMode( EFillMode FillMode )
{
	m_FillMode = FillMode;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetIsTwoSided( bool IsTwoSided )
{
	m_IsTwoSided = IsTwoSided;
	BuildPipelineState();
}

FORCEINLINE void FMaterial::SetMaterialGUID( const FGUID& NewId )
{
	m_GUID = NewId;
}

FORCEINLINE void FMaterial::SetDebugName( const String& Name )
{
#if HE_DEBUG
	m_DebugName = Name;
#endif
}

FORCEINLINE bool FMaterial::SetTexture( const Char* TextureName, HTexture Texture )
{
	StringHashValue NameHash = StringHash( TextureName );
	auto Iter = m_TextureMappings.find( NameHash );
	if (Iter != m_TextureMappings.end())
	{
		(*Iter).second.second = Texture;
		return true;
	}
	HE_LOG( Warning, TEXT( "Trying to set texture with in material \"%s\" with invalid semantic name: %s. Is the Semantic being optimized out?" ), CharToTChar( m_DebugName ), CharToTChar( TextureName ) );
	return false;
}

template <typename T>
FORCEINLINE bool FMaterial::SetShaderFloatVar( const Char* VariableName, const T& Value )
{
	StringHashValue NameHash = StringHash( VariableName );
	auto& Iter = m_ConstBufferVarMappings.find( NameHash );
	if (Iter != m_ConstBufferVarMappings.end())
	{
		for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
		{
			// Copy the data to each frame buffer inside m_ConstBufferMappings.
			memcpy( Iter->second[i].second, &Value, sizeof( T ) );

			// Mark the buffer dirty to indicate a re-upload to the Gpu.
			Iter->second[i].first->m_IsDirty = true;
		}
		return true;
	}
	HE_LOG( Warning, TEXT( "Trying to set buffer constant in material \"%s\" with invalid semantic name: %s" ), CharToTChar( m_DebugName ), CharToTChar( VariableName ) );
	return false;
}

FORCEINLINE bool FMaterial::SetFloat( const Char* VariableName, const float& Value )
{
	return SetShaderFloatVar<float>( VariableName, Value );

}

FORCEINLINE bool FMaterial::SetVector2( const Char* VariableName, const FVector2& Value )
{
	return SetShaderFloatVar<FVector2>( VariableName, Value );
}

FORCEINLINE bool FMaterial::SetVector3( const Char* VariableName, const FVector3& Value )
{
	return SetShaderFloatVar<FVector3>( VariableName, Value );
}

FORCEINLINE bool FMaterial::SetVector4( const Char* VariableName, const FVector4& Value )
{
	return SetShaderFloatVar<FVector4>( VariableName, Value );
}


// FMaterialInstance
//

FORCEINLINE FMaterialInstance::FMaterialInstance()
	: m_InstanceGUID( FGUID::Invalid )
{

}

FORCEINLINE FMaterialInstance::~FMaterialInstance()
{

}
