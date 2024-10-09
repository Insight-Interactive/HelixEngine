#pragma once

#include "Engine/Renderer/ManagedAsset.h"

namespace FMOD
{
	class Sound;
}

class FAudioClip : public ManagedAsset<FAudioClip>
{
	friend class FAudioManager;
protected:
	FAudioClip();
	virtual ~FAudioClip();
public:
	bool Play();
	bool Stop();
	bool Pause();
	bool IsPlaying();
	uint32 GetLength();

private:
	FMOD::Sound* m_Sound = nullptr;
};

typedef ManagedAsset<FAudioClip> ManagedAudioClip;
typedef AssetRef<FAudioClip> HAudioClip;
