// Copyright 2021 Insight Interactive. All Rights Reserved.
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
			pMaterial = new FMaterial();
			pMaterial->SetName( Name );
			m_MaterialCache[Name].reset(pMaterial);
			
			pMaterial->GetAsset().LoadFromFile( Path );
			pMaterial->GetAsset().SetDebugName( Name );
			pMaterial->SetLoadCompleted(true);
		}

	}
	return GetMaterialByName( Name );
}

HMaterial MaterialManager::CreateOneOffMaterial( const String& Path )
{
	ManagedMaterial* pMaterial = NULL;

	String Name = StringHelper::GetFilenameFromDirectoryNoExtension( Path );
	pMaterial = new FMaterial();
	pMaterial->SetName( Name );
	pMaterial->GetAsset().LoadFromFile( Path );
	pMaterial->GetAsset().SetDebugName( Name );
	pMaterial->SetLoadCompleted( true );

	return pMaterial;
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
