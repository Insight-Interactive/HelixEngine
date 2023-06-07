#pragma once

#include "CoreFwd.h"

#include "Font.h"



typedef ManagedAsset<FFont> ManagedFont;
typedef AssetRef<FFont> HFont;

class FFontManager
{
	friend class FRenderingSubsystem;
	friend class FRenderContext;
	friend class HEngine;
public:
	FFontManager()
	{
	}
	virtual ~FFontManager()
	{
	}

	HFont LoadFont( const String& Filename );
	void DestroyFont( const String& Key );

	HFont GetFont( const String& Key )
	{
		return FindOrLoadFont( Key );
	}
private:
	ManagedFont* FindOrLoadFont( const String& Filename );

protected:
	void Initialize();
	void UnInitialize();

	CriticalSection m_Mutex;

protected:
#if R_WITH_D3D12
	std::unordered_map< String, std::unique_ptr<ManagedFont> > m_FontCache;
#endif

};

extern FFontManager GFontManager;
