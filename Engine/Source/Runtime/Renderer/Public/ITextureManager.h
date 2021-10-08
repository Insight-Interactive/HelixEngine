#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CriticalSection.h"
#include "CommonEnums.h"
#include "DataTypeWrappers.h"

class HTexture;

class RENDER_API HManagedTexture
{
	friend class HTextureRef;

public:
	void WaitForLoad() const;
	virtual void CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool sRGB) = 0;

	const String& GetCacheKey() const { return m_MapKey; }

protected:
	HManagedTexture(const std::string& Path)
		: m_MapKey(Path)
	{
	}

protected:
	bool IsValid(void) const { return m_IsValid; }
	void Unload();

	std::string m_MapKey;		// For deleting from the map later
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

	virtual HTextureRef LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB) = 0;
	virtual void DestroyTexture(const String& Key) = 0;

private:
	virtual HManagedTexture* FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB) = 0;

	void DestroyDefaultTextures();

protected:
	FTextureManager() = default;
	virtual ~FTextureManager() = default;

	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;


	CriticalSection m_Mutex;
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
