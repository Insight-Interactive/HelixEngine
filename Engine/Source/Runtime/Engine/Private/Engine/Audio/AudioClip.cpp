#include "EnginePCH.h"

#include "Engine/Audio/AudioClip.h"
#include "Engine/Audio/AudioManager.h"


extern FAudioManager GAudioManager;

FAudioClip::FAudioClip()
	: m_Sound( nullptr )
{
}

FAudioClip::~FAudioClip()
{
}

bool FAudioClip::Play()
{
	FMOD_RESULT result;
	FMOD::System* System = GAudioManager.GetSystem();

	result = System->playSound( m_Sound, nullptr, false, nullptr );
	return result == FMOD_OK;
}

bool FAudioClip::Stop()
{
	FMOD_RESULT result;
	FMOD::System* System = GAudioManager.GetSystem();

	return false;
}

bool FAudioClip::Pause()
{
	return false;
}

bool FAudioClip::IsPlaying()
{
	return false;
}

uint32 FAudioClip::GetLength()
{
	unsigned int SoundLength = 0;
	m_Sound->getLength( &SoundLength, 0 );

	return (uint32)SoundLength;
}
