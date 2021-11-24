#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CriticalSection.h"
#include "CommonEnums.h"
#include "DataTypeWrappers.h"
#include "Texture.h"


class RENDER_API HManagedTexture : public HTexture
{
	friend class HTextureRef;
	friend class FTextureManager;
public:
	void WaitForLoad() const;
	void CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool sRGB);

	const String& GetCacheKey() const { return m_MapKey; }

protected:
	HManagedTexture(const String& Path)
		: m_MapKey(Path)
	{
	}

protected:
	bool IsValid(void) const { return m_IsValid; }
	void Unload();

	String m_MapKey;		// For deleting from the map later
	bool m_IsValid;
	bool m_IsLoading;
	size_t m_ReferenceCount;
};

class RENDER_API FTextureManager
{
	friend class FRenderContextFactory;
	friend class D3D12RenderContextFactory;
	friend class FRenderContext;
public:
	FTextureManager()
	{
	}
	virtual ~FTextureManager()
	{
	}

	HTextureRef LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB);
	void DestroyTexture(const String& Key);

private:
	HManagedTexture* FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB);

	void DestroyDefaultTextures();

protected:

	void Initialize();
	void UnInitialize();

	CriticalSection m_Mutex;

protected:
#if R_WITH_D3D12
	std::unordered_map<String, std::unique_ptr<HManagedTexture>> m_TextureCache;
#endif

};


//
// A handle to a ManagedTexture.  Constructors and destructors modify the reference
// count.  When the last reference is destroyed, the TextureManager is informed that
// the texture should be deleted.
//
class HTextureRef
{
public:
	HTextureRef(const HTextureRef& ref);
	HTextureRef(HManagedTexture* tex = nullptr);
	virtual ~HTextureRef();
	HTextureRef& operator=(const HTextureRef& Other)
	{
		this->m_Ref = Other.m_Ref;
		return *this;
	}
	const std::string& GetCacheKey() const
	{
		return m_Ref->m_MapKey;
	}

	void operator= (std::nullptr_t);
	void operator= (HTextureRef& rhs);

	// Check that this points to a valid texture (which loaded successfully)
	bool IsValid() const;

	// Get the texture pointer.  Client is responsible to not dereference
	// null pointers.
	const HTexture* Get() const;

	const HTexture* operator->() const;

private:
	HManagedTexture* m_Ref;
};
