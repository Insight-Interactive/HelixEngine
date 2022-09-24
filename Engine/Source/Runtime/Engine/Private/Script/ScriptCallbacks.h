#pragma once

#include "lua.hpp"


static int Scr_GetDeltaTime( lua_State* pSourceCtx )
{
	double DeltaTime = GEngine->GetDeltaTime();
	lua_pushnumber( pSourceCtx, DeltaTime );
	return 1;
}

static int Scr_GetAppSeconds( lua_State* pSourceCtx )
{
	double Seconds = GEngine->GetAppSeconds();
	lua_pushnumber( pSourceCtx, Seconds );
	return 1;
}
