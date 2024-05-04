// Copyright 2024 Insight Interactive. All Rights Reserved.
/*

	Verifies configuration is valid for the current build. 
*/
#include "EnginePCH.h"


#if defined HE_WITH_EDITOR && defined HE_GAME_CONSOLE
#	error "[Helix Build Config Validation] - Cannot compile editor for consoles! Editor is only available for desktop configurations."
#endif
