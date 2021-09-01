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

HMaterial* MaterialRef::Get()
{
	return (HMaterial*)m_Ref;
}

HMaterial* MaterialRef::operator->()
{
	HE_ASSERT(m_Ref != nullptr);
	return (HMaterial*)m_Ref;
}


//
// Material Manager Implementation
//

void MaterialManager::LoadMaterialFromFile(const String& Path)
{
	ManagedMaterial* pMat = NULL;

	{
		m_MapMutex.Enter();

		String Name = StringHelper::GetFilenameFromDirectoryNoExtension(Path);
		uint64 HashName = std::hash<String>{}(Name);

		auto Iter = m_MaterialCache.find(Name);
		if (Iter != m_MaterialCache.end())
		{
			pMat = Iter->second.get();
			pMat->WaitForLoad();
			return;
		}
		else
		{
			pMat = new ManagedMaterial(Name);
			// TODO load from file here.
			m_MaterialCache[Name].reset(pMat);
		}

		//DataBlob Data = FileSystem::ReadRawData(Path.c_str());
		//pMat->CreateFromMemory(Data);
		pMat->Initialize();
		pMat->SetDebugName(Name);
		pMat->SetUID(HashName);
		pMat->SetLoadCompleted(true);

		m_MapMutex.Exit();
	}
}

void MaterialManager::DestroyMaterial(const String& Key)
{
	auto Iter = m_MaterialCache.find(Key);
	if (Iter != m_MaterialCache.end())
	{
		(*Iter).second.get()->UnInitialize();
		m_MaterialCache.erase(Iter);
	}
}
