// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "ManagedAsset.h"
#include "Animation.h"


typedef ManagedAsset<FAnimation> ManagedAnimation;
typedef AssetRef<FAnimation> HAnimation;

class RENDER_API FAnimationManager
{
public:
	FAnimationManager()
	{
	}
	~FAnimationManager()
	{
	}

	HAnimation LoadAnimation( const String& Filename );

private:
	ManagedAnimation* FindOrLoadAnimationMeshFromFile( const String& FilePath );
	
private:
	CriticalSection m_Mutex;
	std::unordered_map<String, std::unique_ptr<ManagedAnimation>> m_AnimationCache;

};
