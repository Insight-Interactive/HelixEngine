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


class FCommandContext;

typedef uint64 MaterialID;

enum EShadingModel
{
	SM_DefaultLit	= 0, // Is lit by lights inside the world. Material will be used in Physically-Based lighting.
	SM_Unlit		= 1, // TODO: Implement unlit light pass. Is not lit by lights inside the world.
	SM_Foliage		= 2, // Tree leaves and grass.
};

enum EMaterialDomain
{
	MD_Surface		= 0, // Default material that will be placed in the world.
};

enum EMaterialBlendMode
{
	MBM_Opaque		= 0, // Replace whatever color is currently in the render target when this material is rendered.
	// TODO: MDM_Masked		= 1, //
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

	const String& GetDebugName() const;
	MaterialID GetUID() const;
	HTextureRef GetAlbedoTexture() const;
	HTextureRef GetNormalTexture() const;
	EMaterialBlendMode GetBlendMode() const;
	EMaterialDomain GetDomain() const;
	EShadingModel GetShadingModel() const;
	bool GetIsTwoSided() const;
	void SetColor( FVector4& Color );
	void SetAlbedoTexture( HTextureRef HTextureRef );
	void SetNormalTexture( HTextureRef HTextureRef );
	void SetBlendMode( EMaterialBlendMode Type );
	void SetDomain(EMaterialDomain Domain);
	void SetShadingModel(EShadingModel Model);
	void SetIsTwoSided(bool IsTwoSided);

protected:
	void BindDefaultLit(FCommandContext& GfxContext);
	void BindFoliage(FCommandContext& GfxContext);

	void SetMaterialID( const MaterialID& NewId );
	void SetDebugName( const String& Name );
	void BuildPipelineState();

private:
	MaterialID m_UID;
#if HE_DEBUG
	String m_DebugName;
#endif

	TConstantBuffer<MaterialConstantsCBData> m_ConstantsCB;
	HTextureRef m_AlbedoTexture;
	HTextureRef m_NormalTexture;
	FGUID		m_PixelShaderGuid;
	FGUID		m_VertexShaderGuid;

	EMaterialBlendMode	m_BlendMode;
	EMaterialDomain		m_Domain;
	EShadingModel		m_ShadingModel;
	bool				m_IsTwoSided;

	FPipelineState m_Pipeline;
	FRootSignature m_RootSig;

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

FORCEINLINE HTextureRef FMaterial::GetAlbedoTexture() const
{
	return m_AlbedoTexture;
}

FORCEINLINE HTextureRef FMaterial::GetNormalTexture() const
{
	return m_NormalTexture;
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

FORCEINLINE void FMaterial::SetColor( FVector4& Color )
{
	m_ConstantsCB.GetBufferPointer()->kColor = Color;
}

FORCEINLINE void FMaterial::SetAlbedoTexture( HTextureRef HTextureRef )
{
	m_AlbedoTexture = HTextureRef;
}

FORCEINLINE void FMaterial::SetNormalTexture( HTextureRef HTextureRef )
{
	m_NormalTexture = HTextureRef;
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
