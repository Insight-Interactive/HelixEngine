#pragma once

#include "Engine/Audio/AudioClip.h"


extern FAudioManager GAudioManager;

namespace FMOD
{
	class System;
}

class FAudioManager
{
public:
	FAudioManager();
	~FAudioManager();

	bool Initialize();
	bool Uninitialize();

	FMOD::System& GetSystem() { return *m_System; }

	bool AudioClipExists( const String& Name ) const;
	HAudioClip GetAudioClipByName( const String& Name );

	HAudioClip LoadAudioFile( const FPath& FilePath );

private:
	FMOD::System* m_System;

	CriticalSection m_MapMutex;
	std::unordered_map<String, HAudioClip> m_AudioCache;
};


// Inline function implementations
//

inline bool FAudioManager::AudioClipExists( const String& Name ) const
{
	auto Iter = m_AudioCache.find( Name );
	return Iter != m_AudioCache.end();
}

inline HAudioClip FAudioManager::GetAudioClipByName( const String& Name )
{
	auto Iter = m_AudioCache.find( Name );
	if (Iter != m_AudioCache.end())
	{
		return m_AudioCache.at( Name );
	}
	else
	{
		HE_ASSERT( false );
	}

	return nullptr;
}