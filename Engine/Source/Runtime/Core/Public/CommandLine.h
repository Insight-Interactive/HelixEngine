#pragma once

#include "CoreFwd.h"


class CommandLine
{
public:
	CommandLine();
	~CommandLine();

	void Process( WChar* CommandLine );

	WString& operator[]( const WString& Key );
	bool CommandExists( const WString& Key );

private:
	std::map<WString, WString> m_CommandLineArgs;
};


//
// Inline function implementations
//

inline WString& CommandLine::operator[]( const WString& Key )
{
	return m_CommandLineArgs[Key];
}

inline bool CommandLine::CommandExists( const WString& Key )
{
	return m_CommandLineArgs.find( Key ) != m_CommandLineArgs.end();
}
