#include "EnginePCH.h"

#include "Renderer/FontManager.h"

#include "StringHelper.h"

FFontManager GFontManager;


HFont FFontManager::LoadFont( const String& Filename )
{
	return FindOrLoadFont( Filename );
}

void FFontManager::DestroyFont( const String& Key )
{

}

ManagedFont* FFontManager::FindOrLoadFont( const String& Filename )
{
	ManagedFont* pFont = NULL;

	String Key = StringHelper::GetFilenameFromDirectoryNoExtension( Filename );

	{
		ScopedCriticalSection Guard( m_Mutex );


		// Search for an existing managed font
		auto iter = m_FontCache.find( Key );
		if (iter != m_FontCache.end())
		{
			// If a font was already created make sure it has 
			// finished loading before returning a pointer to it.
			pFont = iter->second.get();
			pFont->WaitForLoad();
			return pFont;
		}
		else
		{
			pFont = new FFont();
			pFont->SetName( Key );
			m_FontCache[Key].reset( pFont );

			pFont->GetAsset().Initialize( Filename );
			pFont->SetLoadCompleted( true );
		}
	}

	return pFont;
}

void FFontManager::Initialize()
{

}

void FFontManager::UnInitialize()
{

}
