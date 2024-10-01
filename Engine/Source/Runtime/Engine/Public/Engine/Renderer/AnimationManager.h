// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "Engine/Renderer/Animation.h"
#include "Engine/Renderer/ManagedAsset.h"


class RENDER_API FAnimationManager
{
public:
	FAnimationManager()
	{
	}
	~FAnimationManager()
	{
	}

	HAnimation LoadAnimation( const FPath& Filename );

private:
	ManagedAnimation* FindOrLoadAnimationMeshFromFile( const FPath& FilePath );

private:
	CriticalSection m_Mutex;
	std::unordered_map<String, std::unique_ptr<ManagedAnimation>> m_AnimationCache;

};
