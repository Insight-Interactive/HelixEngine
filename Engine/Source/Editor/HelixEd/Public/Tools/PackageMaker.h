#pragma once

enum EBuildPlatform
{
	Prospero,
	Win32,
	Win64,
	Durango,
	Scorpio,

	BuildPlatform_Count,
};

enum EBuildConfig
{
	DebugGame,
	ShippingGame,

	BuildConfig_Count,
};

extern const Char* kBuildPlatformStrings[BuildPlatform_Count];

extern const Char* kBuildConfigStrings[BuildConfig_Count];

class PackageMaker
{
public:
	/*
		Retuns true if visual studio is installed in the default directory, false if not.
	*/
	static bool CheckVisualStudioInstalled();

	static bool BuildPackage( EBuildConfig BuildConfig, EBuildPlatform BuildPlatform, const Char* GameName, const Char* ProjectDir, const Char* BuildDir );

	static const Char* GetBuildPlatformString( EBuildPlatform BuildPlatform );
	
	static const Char* GetBuildConfigString( EBuildConfig BuildConfig );

};


//
// Inline function implamentations
//

inline /*static*/ const Char* PackageMaker::GetBuildPlatformString( EBuildPlatform BuildPlatform )
{
	HE_ASSERT( BuildPlatform != BuildPlatform_Count );
	return kBuildPlatformStrings[BuildPlatform];
}

inline /*static*/ const Char* PackageMaker::GetBuildConfigString( EBuildConfig BuildConfig )
{
	HE_ASSERT( BuildConfig != BuildConfig_Count );
	return kBuildConfigStrings[BuildConfig];
}
