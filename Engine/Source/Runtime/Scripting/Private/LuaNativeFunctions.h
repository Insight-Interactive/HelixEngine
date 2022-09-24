#pragma once


struct lua_State;

/*
	Lua -> C
	Logs a string to the C console.
*/
static int Scr_DebugLog( lua_State* SourceContext )
{
#if HE_DEBUG
	const char* UserMsg = lua_tostring( SourceContext, -1 );
	S_LOG( Log, TEXT("%s"), CharToTChar( UserMsg ) );
#endif
	return 0;
}
