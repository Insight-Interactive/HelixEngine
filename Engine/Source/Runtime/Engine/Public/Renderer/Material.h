#pragma once

#include "RendererCore.h"
#include "ITextureManager.h"

#include "Renderer/Common.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/ConstantBufferStructures.h"

class ICommandContext;

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

	void Bind(ICommandContext& GfxContext);
	void CreateFromMemory(DataBlob Memory);
	void WriteToFile();


	FVector4 GetColor() const
	{
		return m_ConstantsData.Color;
	}

	TextureRef GetAlbedoTexture() const
	{
		return m_AlbedoTexture;
	}

	TextureRef GetNormalTexture() const
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

	void SetAlbedoTexture(TextureRef TextureRef)
	{
		m_AlbedoTexture = TextureRef;
	}

	void SetNormalTexture(TextureRef TextureRef)
	{
		m_NormalTexture = TextureRef;
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
	void BindOpaque(ICommandContext& GfxContext);

	void BindTranslucent(ICommandContext& GfxContext);

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

private:
	IConstantBuffer* m_pConstantsCB;
	MaterialConstantsCBData m_ConstantsData;
	TextureRef m_AlbedoTexture;
	TextureRef m_NormalTexture;
#if IE_DEBUG
	FString m_DebugName;
#endif

	MaterialID m_UID;
	EMaterialType m_Type;
};
