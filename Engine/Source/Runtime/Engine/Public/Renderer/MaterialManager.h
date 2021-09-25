#pragma once

#include "CriticalSection.h"
#include "Renderer/Material.h"


class ManagedMaterial : public HMaterial
{
	friend class MaterialRef;
	friend class MaterialManager;
public:
	ManagedMaterial() = default;
	virtual ~ManagedMaterial() = default;

	void WaitForLoad() const;

protected:
	ManagedMaterial(const String& HashName)
		: m_MapKey(HashName)
		, m_IsValid(false)
		, m_IsLoading(true)
		, m_ReferenceCount(1)
	{
	}
	void SetLoadCompleted(bool Completed)
	{
		m_IsLoading = !Completed;
		m_IsValid = Completed;
	}

protected:
	bool IsValid(void) const { return GetAlbedoTexture().IsValid() && GetNormalTexture().IsValid(); }
	void Unload();

	String m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;
	uint64 m_ReferenceCount;
};

class MaterialRef
{
public:
	MaterialRef(const MaterialRef& ref);
	MaterialRef(ManagedMaterial* pTex = nullptr);
	~MaterialRef();
	MaterialRef& operator=(const MaterialRef& Other)
	{
		this->m_Ref = Other.m_Ref;
		return *this;
	}


	void operator= (std::nullptr_t);
	void operator= (MaterialRef& rhs);

	// Check that this points to a valid texture (which loaded successfully)
	bool IsValid() const;

	// Get the texture pointer.  Client is responsible to not dereference
	// null pointers.
	HMaterial* Get();

	HMaterial* operator->();

private:
	ManagedMaterial* m_Ref;
};



class MaterialManager
{
public:
	MaterialManager() = default;
	~MaterialManager() = default;

	MaterialRef LoadMaterialFromFile(const String& Path);

	void DestroyMaterial(const String& Key);

	MaterialRef GetMaterialByName(const String& Id)
	{
		return m_MaterialCache.at(Id).get();
	}

private:
	std::map< String, std::unique_ptr<ManagedMaterial> > m_MaterialCache;

	CriticalSection m_MapMutex;
};
