#pragma once

#include "RendererFwd.h"

#include "ManagedAsset.h"
#include "Transform.h"


struct FAnimationKeyFrame
{
	float m_Timestamp;
	FTransform m_AnimatedTransform;
};


class RENDER_API FAnimation : public ManagedAsset<FAnimation>
{
public:
	double m_Duration;
	double m_TicksPerSecond;

	String m_Name;
	float m_Length;
	std::map<String, std::vector<FAnimationKeyFrame> > m_KeyFrames; // <JointName, Corespoding keyframes>

};
