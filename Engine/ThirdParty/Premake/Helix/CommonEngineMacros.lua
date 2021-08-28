-- Copyright 2021 Insight Interactive. All Rights Reserved.


--
-- Platform
--

filter ("platforms:Win32")
	defines
	{
		"HE_WIN32",
		"HE_WINDOWS_DESKTOP",
	}

filter ("platforms:Win64")
	defines
	{
		"HE_WIN64",
		"HE_WINDOWS_DESKTOP",
	}

filter ("platforms:XboxOne")
	defines	
	{
		"HE_WINDOWS_UNIVERSAL",
		"HE_XBOX_ONE",
		"HE_GAME_CONSOLE",
	}

filter ("platforms:XboxOneX")
	defines	
	{
		"HE_WINDOWS_UNIVERSAL",
		"HE_XBOX_ONE_X",
		"HE_GAME_CONSOLE",
	}

filter ("platforms:Win32 or Win64 or XboxOne or XboxOneX")
	defines
	{
		"HE_WINDOWS",
	}

filter ("platforms:Playstation5")
	defines
	{
		"HE_PLAYSTATION",
		"HE_PS5",
		"HE_GAME_CONSOLE",
	}


--
-- Common
--

filter ("configurations:DebugEditor or Development or DebugGame")
	defines
	{
		"HE_ENABLE_LOGS",
		"HE_ENABLE_ASSERTS",
	}

filter ("configurations:DebugEditor")
	defines
	{
		"HE_DEBUG_THREADS",
		"HE_DEBUG_FILESYSTEM",
	}