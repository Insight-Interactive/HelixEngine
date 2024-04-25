#pragma once

#include "CoreFwd.h"


class FCommandLine
{
public:
	FCommandLine();
	~FCommandLine();

	void Process( const WChar* CommandLine );

	const WString& operator[]( const WString& Key );
	bool CommandExists( const WString& Key );
	bool ArgumentEquals( const WString& Key, const WString& Value );

	void AppendArg( const WString& Key, const WString& Value );

private:
	std::map<WString, WString> m_CommandLineArgs;
};


//
// Inline function implementations
//

FORCEINLINE const WString& FCommandLine::operator[]( const WString& Key )
{
	return m_CommandLineArgs[Key];
}

FORCEINLINE bool FCommandLine::CommandExists( const WString& Key )
{
	return m_CommandLineArgs.find( Key ) != m_CommandLineArgs.end();
}

FORCEINLINE bool FCommandLine::ArgumentEquals( const WString& Key, const WString& Value ) 
{
	return m_CommandLineArgs[Key] == Value;
}

FORCEINLINE void FCommandLine::AppendArg( const WString& Key, const WString& Value )
{
	m_CommandLineArgs.emplace( Key, Value );
}
