// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

/*
	Encapculats the source code of a lua script file.
*/
class SCRIPT_API LuaScript
{
public:
	LuaScript();
	LuaScript( const char* Filename );
	~LuaScript();
	LuaScript( const LuaScript& Other );
	
	// Load the lua source code from file.
	void LoadSource( const char* Filename );

	// Returns the raw source code for the script.
	const char* GetSourceCode();
	// Returns tha name of the script file.
	String GetScriptName();

private:
	FileRef m_SourceCode;

};
typedef std::shared_ptr<LuaScript> LuaScriptRef;
