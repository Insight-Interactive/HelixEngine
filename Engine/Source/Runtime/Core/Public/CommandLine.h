#pragma once

#include "CoreFwd.h"


class FCommandLine
{
public:
	FCommandLine();
	~FCommandLine();

	void Process( const WChar* CommandLine );

	WString& operator[]( const WString& Key );
	bool CommandExists( const WString& Key );

private:
	std::map<WString, WString> m_CommandLineArgs;
};


//
// Inline function implementations
//

inline WString& FCommandLine::operator[]( const WString& Key )
{
	return m_CommandLineArgs[Key];
}

inline bool FCommandLine::CommandExists( const WString& Key )
{
	return m_CommandLineArgs.find( Key ) != m_CommandLineArgs.end();
}
