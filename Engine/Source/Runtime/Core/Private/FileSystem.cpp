#include "CorePCH.h"

#include "FileSystem.h"
#include "System.h"
#include "DataTypeWrappers.h"

/* static */ bool FileSystem::Init()
{
	// Set the working directory of the application no matter what environment we are running in.
	TChar Path[HE_MAX_PATH];
	System::GetWorkingDirectory( HE_MAX_PATH, Path );
	System::SetWorkingDirectory( Path );

	return true;
}

/*static*/ DataBlob FileSystem::ReadRawData(const char* Path)
{
	DataBlob Data;
	
	FILE* pFile = NULL;
	fopen_s(&pFile, Path, "rb");
	if (pFile == NULL)
	{
		Data.Invalidate();
		return Data;
	}

	// Get the size of the file.
	fseek(pFile, 0, SEEK_END);
	Data.m_DataSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// Fill the buffer with the data in the file.
	Data->Reserve(Data.m_DataSize);
	fread(Data.GetBufferPointer(), 1, Data.m_DataSize, pFile);

	fclose(pFile);

	return Data;
}

/*static*/ bool FileSystem::DoesFileExist( const char* Filepath )
{
	FILE* Result = NULL;
	fopen_s( &Result, Filepath, "r" );
	if (Result != NULL)
	{
		fclose( Result );
		return true;
	}
	return false;
}
