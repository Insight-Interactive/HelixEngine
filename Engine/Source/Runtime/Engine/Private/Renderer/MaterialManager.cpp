// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/MaterialManager.h"
#include "StringHelper.h"


void ManagedMaterial::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
		std::this_thread::yield();
}

void ManagedMaterial::Unload()
{
	GMaterialManager.DestroyMaterial(m_MapKey);
}

MaterialRef::MaterialRef(const MaterialRef& ref)
	: m_Ref(ref.m_Ref)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

MaterialRef::MaterialRef(ManagedMaterial* tex)
	: m_Ref(tex)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

MaterialRef::~MaterialRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

void MaterialRef::operator= (std::nullptr_t)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

void MaterialRef::operator= (MaterialRef& rhs)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

bool MaterialRef::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

FMaterial* MaterialRef::Get()
{
	return (FMaterial*)m_Ref;
}

FMaterial* MaterialRef::operator->()
{
	HE_ASSERT(m_Ref != nullptr);
	return (FMaterial*)m_Ref;
}

const FMaterial* MaterialRef::operator->() const
{
	HE_ASSERT( m_Ref != nullptr );
	return (const FMaterial*)m_Ref;
}


//
// Material Manager Implementation
//

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

MaterialRef MaterialManager::FindOrLoadMaterialFromFile(const String& Path)
{
	ManagedMaterial* pMaterial = NULL;

	String Name = StringHelper::GetFilenameFromDirectoryNoExtension( Path );
	{
		ScopedCriticalSection Guard( m_MapMutex );

		auto Iter = m_MaterialCache.find(Name);
		if (Iter != m_MaterialCache.end())
		{
			pMaterial = Iter->second.get();
			pMaterial->WaitForLoad();
		}
		else
		{
			pMaterial = new ManagedMaterial(Name);
			m_MaterialCache[Name].reset(pMaterial);

			pMaterial->LoadFromFile( Path );
			pMaterial->SetDebugName(Name);
			pMaterial->SetLoadCompleted(true);
		}

	}
	return GetMaterialByName( Name );
}

MaterialRef MaterialManager::CreateOneOffMaterial( const String& Path )
{
	ManagedMaterial* pMaterial = NULL;

	String Name = StringHelper::GetFilenameFromDirectoryNoExtension( Path );
	pMaterial = new ManagedMaterial( Name );
	pMaterial->LoadFromFile( Path );
	pMaterial->SetDebugName( Name );
	pMaterial->SetLoadCompleted( true );

	return pMaterial;
}

void MaterialManager::DestroyMaterial(const String& Key)
{
	ScopedCriticalSection Guard(m_MapMutex);

	auto Iter = m_MaterialCache.find(Key);
	if (Iter != m_MaterialCache.end())
	{
		Iter->second->Destroy();
		m_MaterialCache.erase(Iter);
	}
}

void MaterialManager::FlushMaterialCache()
{
	for(auto& Iter : m_MaterialCache)
	{
		Iter.second->Destroy();
	}
	m_MaterialCache.clear();
}
