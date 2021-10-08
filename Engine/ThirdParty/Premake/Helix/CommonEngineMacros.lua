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

filter ("platforms:Durango")
	defines	
	{
		"HE_WINDOWS_UNIVERSAL",
		"HE_XBOX",
		"HE_DURANGO",
		"HE_GAME_CONSOLE",
	}

filter ("platforms:Scorpio")
	defines	
	{
		"HE_WINDOWS_UNIVERSAL",
		"HE_XBOX",
		"HE_SCORPIO",
		"HE_GAME_CONSOLE",
	}

filter ("platforms:Win32 or Win64 or Durango or Scorpio")
	defines
	{
		"HE_WINDOWS",
	}

filter ("platforms:Prospero")
	defines
	{
		"HE_PLAYSTATION",
		"HE_PROSPERO",
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

filter ("configurations:DebugGame or ShippingGame")
	defines
	{
		"HE_STANDALONE",
	}