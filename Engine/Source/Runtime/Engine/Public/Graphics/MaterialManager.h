// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CriticalSection.h"
#include "Graphics/Material.h"
#include "Engine/Renderer/ManagedAsset.h"


typedef ManagedAsset<FMaterial> ManagedMaterial;
typedef AssetRef<FMaterial> HMaterial;

class MaterialManager
{
	friend class HEngine;
public:
	MaterialManager();
	~MaterialManager();

	HMaterial FindOrLoadMaterialFromFile(const String& Path);
	void DestroyMaterial(const String& Key);
	HMaterial GetMaterialByName( const String& Id );
	void FlushMaterialCache();

private:
	std::unordered_map< String, std::unique_ptr<ManagedMaterial> > m_MaterialCache;
	CriticalSection m_MapMutex;

};

// Inline function implementations
//

FORCEINLINE HMaterial MaterialManager::GetMaterialByName( const String& Id )
{
	return m_MaterialCache.at( Id ).get();
}
