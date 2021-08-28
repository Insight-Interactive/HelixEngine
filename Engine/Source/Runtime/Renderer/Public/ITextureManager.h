#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CriticalSection.h"
#include "CommonEnums.h"
#include "DataTypeWrappers.h"

class ITexture;

class RENDER_API IManagedTexture
{
	friend class TextureRef;

public:
	void WaitForLoad() const;
	virtual void CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool sRGB) = 0;

	const String& GetCacheKey() const { return m_MapKey; }

protected:
	IManagedTexture(const std::string& Path)
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

class RENDER_API ITextureManager
{
	friend class IRenderContextFactory;
	friend class D3D12RenderContextFactory;
	friend class RenderContext;
public:

	virtual TextureRef LoadTexture(const std::string& FileName, EDefaultTexture Fallback, bool forceSRGB) = 0;
	virtual void DestroyTexture(const std::string& Key) = 0;

private:
	virtual IManagedTexture* FindOrLoadTexture(const std::string& FileName, EDefaultTexture Fallback, bool forceSRGB) = 0;

	void DestroyDefaultTextures();

protected:
	ITextureManager() = default;
	virtual ~ITextureManager() = default;

	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;


	CriticalSection m_Mutex;
};


//
// A handle to a ManagedTexture.  Constructors and destructors modify the reference
// count.  When the last reference is destroyed, the TextureManager is informed that
// the texture should be deleted.
//
class TextureRef
{
public:

	TextureRef(const TextureRef& ref);
	TextureRef(IManagedTexture* tex = nullptr);
	virtual ~TextureRef();
	TextureRef& operator=(const TextureRef& Other)
	{
		this->m_Ref = Other.m_Ref;
		return *this;
	}
	const std::string& GetCacheKey() const
	{
		return m_Ref->m_MapKey;
	}

	void operator= (std::nullptr_t);
	void operator= (TextureRef& rhs);

	// Check that this points to a valid texture (which loaded successfully)
	bool IsValid() const;

	// Get the texture pointer.  Client is responsible to not dereference
	// null pointers.
	const ITexture* Get() const;

	const ITexture* operator->() const;

private:
	IManagedTexture* m_Ref;
};
