#pragma once

#include "RendererCore.h"
#include "ITextureManager.h"

#include "Renderer/Common.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/ConstantBufferStructures.h"

class FCommandContext;

typedef uint64 MaterialID;


class HMaterial
{
	friend class MaterialManager;

	enum EMaterialType
	{
		MT_Opaque,
		MT_Translucent,
	};

public:
	HMaterial()
		: m_UID(HE_INVALID_MATERIAL_ID)
		, m_Type(MT_Opaque)
	{
	}
	~HMaterial()
	{
	}

	void Bind(FCommandContext& GfxContext);
	
	void LoadFromFile( const String& Filepath );
	

	FVector4 GetColor() const
	{
		return m_ConstantsData.Color;
	}

	HTextureRef GetAlbedoTexture() const
	{
		return m_AlbedoTexture;
	}

	HTextureRef GetNormalTexture() const
	{
		return m_NormalTexture;
	}

	EMaterialType GetType() const
	{
		return m_Type;
	}

	void SetColor(FVector4& Color)
	{
		m_ConstantsData.Color = Color;
	}

	void SetAlbedoTexture(HTextureRef HTextureRef)
	{
		m_AlbedoTexture = HTextureRef;
	}

	void SetNormalTexture(HTextureRef HTextureRef)
	{
		m_NormalTexture = HTextureRef;
	}

	void SetType(EMaterialType Type)
	{
		m_Type = Type;
	}

	MaterialID GetUID() const
	{
		return m_UID;
	}

	bool IsValid()
	{
		return m_UID != HE_INVALID_MATERIAL_ID;
	}

protected:
	void BindOpaque(FCommandContext& GfxContext);

	void BindTranslucent(FCommandContext& GfxContext);

	void Initialize();

	void UnInitialize();

	void SetUID(const MaterialID& NewId)
	{
		m_UID = NewId;
	}

	void SetDebugName(const String& Name)
	{
#if IE_DEBUG
		m_DebugName = Name;
#endif
	}

	// WIP
	void CreateFromMemory( DataBlob Memory );
	// WIP
	void WriteToFile();

private:
	FConstantBuffer* m_pConstantsCB;
	MaterialConstantsCBData m_ConstantsData;
	HTextureRef m_AlbedoTexture;
	HTextureRef m_NormalTexture;
#if IE_DEBUG
	FString m_DebugName;
#endif

	MaterialID m_UID;
	EMaterialType m_Type;
};
