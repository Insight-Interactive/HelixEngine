#include "EnginePCH.h"

#include "Engine/Audio/AudioManager.h"

FAudioManager GAudioManager;


FAudioManager::FAudioManager()
	: m_System( nullptr )
{
}

FAudioManager::~FAudioManager()
{
}

bool FAudioManager::Initialize()
{
	FMOD_RESULT result;
	result = FMOD::System_Create( &m_System );      // Create the main system object.
	if (result != FMOD_OK)
	{
		HE_LOG( Error, TEXT( "FMOD error! (%d) %s" ), result, FMOD_ErrorString( result ) );
		HE_ASSERT( false );
	}

	result = m_System->init( 512, FMOD_INIT_NORMAL, 0 );    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		HE_LOG( Error, TEXT( "FMOD error! (%d) %s" ), result, FMOD_ErrorString( result ) );
		HE_ASSERT( false );
	}

	return true;
}

bool FAudioManager::Uninitialize()
{
	if (m_System)
	{
		m_System->close();
		m_System = nullptr;
	}

	return true;
}

HAudioClip FAudioManager::LoadAudioFile( const FPath& FilePath )
{
	FMOD_RESULT result;
	HAudioClip AudioClip = new FAudioClip();

	String FileName = StringHelper::GetFilenameFromDirectoryNoExtension( FilePath.m_Path );
	if (AudioClipExists( FileName ))
		return GetAudioClipByName( FileName );

	result = m_System->createSound( FilePath.m_Path, FMOD_DEFAULT, nullptr, &AudioClip.GetAsset().m_Sound );
	HE_ASSERT( result == FMOD_OK );
	AudioClip->SetLoadCompleted( true );

	ScopedCriticalSection Guard( m_MapMutex );
	m_AudioCache[FileName] = AudioClip;
	return m_AudioCache[FileName];
}
