#include "CorePCH.h"

#include "CommandLine.h"


CommandLine::CommandLine()
{

}

CommandLine::~CommandLine()
{

}

void CommandLine::Process( WChar* CommandLine )
{
	uint64 i = 0;
	bool FoundKey = false;
	bool FoundValue = false;
	WString Key = L"";
	WString Value = L"";

	while (CommandLine[i] != '\0')
	{
		// Found a key.
		if (CommandLine[i] == '-')
		{
			FoundKey = true;
		}

		// Found the space between the key and value.
		// Proceed to process the value.
		if (FoundKey && CommandLine[i] == ' ')
		{
			FoundValue = true;
			FoundKey = false;
			i++;
			continue;
		}

		// Process the key.
		if (FoundKey)
		{
			Key += CommandLine[i];
		}

		// Reached the end of the value. Reset ans start looking for keys again.
		if (FoundValue && CommandLine[i] == ' ' || CommandLine[i + 1] == '\0')
		{
			FoundKey = false;
			FoundValue = false;

			if(CommandLine[i + 1] == '\0')
				Value += CommandLine[i];

			m_CommandLineArgs.emplace( Key, Value );
			Key = L"";
			Value = L"";
			i++;
			continue;
		}

		// Process the value.
		if (FoundValue)
		{
			Value += CommandLine[i];
		}

		i++;
	}
}
