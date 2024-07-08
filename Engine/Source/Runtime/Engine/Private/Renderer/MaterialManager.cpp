// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/MaterialManager.h"
#include "StringHelper.h"


//
// Material Manager Implementation
//

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

HMaterial MaterialManager::FindOrLoadMaterialFromFile(const String& Path)
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

			String Extention = StringHelper::GetFileExtension( Path );
			if (Extention == "hmat")
				pMaterial = new FMaterial();
			else if (Extention == "hmatinst")
				pMaterial = new FMaterialInstance();
			else
				HE_ASSERT( false ); // Asset that was not a material was trying to be loaded by the material manager!

			pMaterial->SetName( Name );
			m_MaterialCache[Name].reset(pMaterial);
			
			pMaterial->GetAsset().LoadFromFile( Path );
			pMaterial->GetAsset().SetDebugName( Name );
			pMaterial->SetLoadCompleted(true);
		}

	}
	return GetMaterialByName( Name );
}

void MaterialManager::DestroyMaterial(const String& Key)
{
	ScopedCriticalSection Guard(m_MapMutex);

	auto Iter = m_MaterialCache.find(Key);
	if (Iter != m_MaterialCache.end())
	{
		Iter->second->GetAsset().Destroy();
		m_MaterialCache.erase(Iter);
	}
}

void MaterialManager::FlushMaterialCache()
{
	for(auto& Iter : m_MaterialCache)
	{
		Iter.second->GetAsset().Destroy();
	}
	m_MaterialCache.clear();
}
