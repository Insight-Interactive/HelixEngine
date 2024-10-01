#pragma once

#include "RendererFwd.h"

#include "ManagedAsset.h"
#include "Transform.h"


struct FAnimationKeyFrame
{
	FAnimationKeyFrame()
		: m_Timestamp( 0.f ) {}

	float m_Timestamp;
	FVector3 Position;
	FQuat Rotation;
};


class RENDER_API FAnimation : public ManagedAsset<FAnimation>
{
	friend class FAnimationManager;
	friend class FAssetDatabase;
public:
	FAnimation()
		: m_Duration( 0.f ), m_TicksPerSecond( 0.f ) {}
	float m_Duration;
	float m_TicksPerSecond;

	String m_Name;
	float m_Length;
	std::map<String, std::vector<FAnimationKeyFrame> > m_KeyFrames; // <JointName, Corespoding keyframes>

};

typedef ManagedAsset<FAnimation> ManagedAnimation;
typedef AssetRef<FAnimation> HAnimation;