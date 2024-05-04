// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"

#include "DataTypeWrappers.h"


class FileSystem
{
public:
	/*
		Initializes the filesystems working directory. Should be called once
		during app initialization.
	*/
	static bool Init();

	/*
		Reads in the raw data for a file and returns its contents. nullptr if the file read was unsuccessful.
		@param Path - Exe relative path to the file to read.
		@param OutDataSize - Populated with the size of the file that is read in. -1 if the file read was unsuccessful.
	*/
	static DataBlob ReadRawData(const char* Path);

	/*
		Returns true if a file exists on disk, false if not.
		@param Filepath - The file to check.
	*/
	static bool DoesFileExist( const char* Filepath );

};


enum EContentMode
{
	/*
		File has not been initialized!
	*/
	CM_Unset = -1,
	
	/*
		Use the file's contents as binary.
	*/
	CM_Binary = 0,

	/*
		Use the file's contents as text.
	*/
	CM_Text = 1,
};

enum EFileUsageMode
{
	/*
		File has not been initialized!
	*/
	FUM_Invalid = -1,

	/*
		Open the file for reading only.
	*/
	FUM_Read = 0,

	/*
		Open the file for writing only. If the file does not exist, a new one will be created.
	*/
	FUM_Write = 1,

	/*
		Same a FUM_Write, but appends data to the end of the file.
	*/
	FUM_Append = 2,
};

class File
{
public:
	File( const Char* pFilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode );
	File( const String& FilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode );
	File();
	~File();

	/*
		Manually load the file. If the file is already loaded it will be unloaded.
	*/
	void Load( const Char* pFilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode );

	/*
		Unloads the file from memory.
	*/
	void Unload();

	/*
		Returns the file size in bytes.
	*/
	uint64 GetSizeInBytes();

	/*
		Writes data to the file. Returns true if all data was successfuly written to the file, false if not.
		@param pData - Pointer to the array of elements to write to the file.
		@param ElementSize - The size in bytes of each element in the array.
		@param ElementCount - The number of elements to write.
	*/
	bool WriteData( void* pData, uint64 ElementSize, uint64 ElementCount );

	/*
		Reads data from the file and returns a pointer to the start of it.
	*/
	void* ReadData();

	/*
		Returns a pointer to the data in the file.
	*/
	void* Data();

	/*
		Returns a const pointer to the data in the file.
	*/
	const void* Data() const;

	/*
		Returns true if the file is open, false if not.
	*/
	bool IsOpen();

	/*
		Returns the path of the file.
	*/
	const Char* GetFilepath() const;


protected:
	FILE* GetCFile();
	const Char* GetCFileUsageMode( const EFileUsageMode& Mode );
	const Char* GetCFileContentUsageMode( const EContentMode& ContentUsageMode );


private:
	FILE* m_pFile;
	void* m_pContents;
	EFileUsageMode m_UsageMode;
	EContentMode m_ContentUsageMode;
	Char m_Filepath[MAX_PATH];
};
using FileRef = Ref<File>;


//
// Inline function implementations
//

// File
//

FORCEINLINE File::File( const Char* pFilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode = CM_Text )
	: m_pFile( null )
	, m_pContents( null )
	, m_UsageMode( UsageMode )
	, m_ContentUsageMode( ContentUsageMode )
{
	Load( pFilePath, UsageMode, ContentUsageMode );
}

FORCEINLINE File::File( const String& Filepath, EFileUsageMode UsageMode, EContentMode ContentUsageMode )
	: File(Filepath.c_str(), UsageMode, ContentUsageMode)
{
}

FORCEINLINE File::File()
	: m_pFile( null )
	, m_pContents( null )
	, m_UsageMode( FUM_Invalid )
	, m_ContentUsageMode( CM_Unset )
{
}

FORCEINLINE File::~File()
{
	Unload();
}

FORCEINLINE const Char* File::GetCFileUsageMode( const EFileUsageMode& Mode )
{
	switch (Mode)
	{
	case FUM_Read:
		return "r";
	case FUM_Write:
		return "w";
	case FUM_Append:
		return "a";
	default:
		HE_ASSERT( false );
		break;
	}
	return "a";
}

FORCEINLINE const Char* File::GetCFileContentUsageMode( const EContentMode& ContentUsageMode )
{
	switch (ContentUsageMode)
	{
	case CM_Binary:
		return "b";
	case CM_Text:
		return ""; // Default is nothing for fopen.
	default:
		HE_ASSERT( false );
		break;
	}

	return "";
}

FORCEINLINE void File::Load( const Char* pFilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode )
{
	if (m_pContents != nullptr)
		Unload();

	m_UsageMode = UsageMode;
	m_ContentUsageMode = ContentUsageMode;
	strcpy_s( m_Filepath, pFilePath );

	Char FileOpMode[4] = {};
	const Char* CUsageMode = GetCFileUsageMode( UsageMode );
	const Char* CContentUsageMode = GetCFileContentUsageMode( ContentUsageMode );
	strcat_s( FileOpMode, CUsageMode );
	strcat_s( FileOpMode, CContentUsageMode );

	fopen_s( &m_pFile, m_Filepath, FileOpMode );
	HE_ASSERT( m_pFile != null );

	switch (UsageMode)
	{
	case FUM_Read:
		ReadData();
		break;
	default:
		break;
	}
}

FORCEINLINE void File::Unload()
{
	if (m_pFile == NULL)
		return; // Should we assert instead?

	fclose( m_pFile );
	if (m_pContents != NULL)
	{
		HE_HeapFree( m_pContents );
		m_pContents = NULL;
	}
	m_ContentUsageMode = CM_Unset;
	m_UsageMode = FUM_Invalid;
}

FORCEINLINE FILE* File::GetCFile()
{
	return m_pFile;
}

FORCEINLINE void* File::Data()
{
	return m_pContents;
}

FORCEINLINE const void* File::Data() const
{
	return (const void*)m_pContents;
}

FORCEINLINE bool File::IsOpen()
{
	return m_pFile != NULL;
}

FORCEINLINE uint64 File::GetSizeInBytes()
{
	uint64 FileSize = 0;
	HE_ASSERT( m_pFile != null );

	fseek( m_pFile, 0, SEEK_END );
	FileSize = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_SET );

	return FileSize;
}

FORCEINLINE bool File::WriteData(void* pData, uint64 ElementSize, uint64 ElementCount)
{
	HE_ASSERT( m_UsageMode == FUM_Write || m_UsageMode == FUM_Append );
	
	return fwrite( pData, (size_t)ElementSize, (size_t)ElementCount, GetCFile() ) == ElementCount;
}

FORCEINLINE void* File::ReadData()
{
	HE_ASSERT( m_UsageMode == FUM_Read ); // Trying to read contents of a file with no read privlages.
	
	uint64 FileSizeInBytes = GetSizeInBytes();
	m_pContents = HE_HeapAlloc( (size_t)FileSizeInBytes );
	HE_ASSERT( m_pContents != null ); // Failed to allocate memory.
	ZeroMemory( m_pContents, (size_t)FileSizeInBytes );

	fread( m_pContents, 1, (size_t)FileSizeInBytes, m_pFile );
	HE_ASSERT( m_pContents != null ); // Failed to read the file contents into the buffer.

	return m_pContents;
}

FORCEINLINE const Char* File::GetFilepath() const
{
	return m_Filepath;
}
