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

};


enum EContentMode
{
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
	~File();

	uint64 GetSizeInBytes();

	void  WriteData( void* pData, uint64 ElementSize, uint64 ElementCount );
	void* ReadData();

	void* Data();
	bool IsOpen();

protected:
	FILE* GetCFile();
	const Char* GetCFileUsageMode( const EFileUsageMode& Mode );
	const Char* GetCFileContentUsageMode( const EContentMode& ContentUsageMode );

private:
	FILE* m_pFile;
	void* m_pContents;
	EFileUsageMode m_UsageMode;
	EContentMode m_ContentUsageMode;
};
using FileRef = Ref<File>;


//
// Inline function implementations
//

// File
//

inline File::File( const Char* pFilePath, EFileUsageMode UsageMode, EContentMode ContentUsageMode )
	: m_pFile( null )
	, m_pContents( null )
	, m_UsageMode( UsageMode )
	, m_ContentUsageMode( ContentUsageMode )
{
	Char FileOpMode[4] = {};
	const Char* CUsageMode = GetCFileUsageMode( UsageMode );
	const Char* CContentUsageMode = GetCFileContentUsageMode( ContentUsageMode );
	strcat_s( FileOpMode, CUsageMode );
	strcat_s( FileOpMode, CContentUsageMode );
	
	fopen_s( &m_pFile, pFilePath, FileOpMode );
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

inline File::~File()
{
	HE_ASSERT( m_pFile != null );
	fclose( m_pFile );
	if (m_pContents != NULL)
	{
		free( m_pContents );
		m_pContents = NULL;
	}
}

inline const Char* File::GetCFileUsageMode( const EFileUsageMode& Mode )
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

inline const Char* File::GetCFileContentUsageMode( const EContentMode& ContentUsageMode )
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

inline FILE* File::GetCFile()
{
	return m_pFile;
}

inline void* File::Data()
{
	return m_pContents;
}

inline bool File::IsOpen()
{
	return m_pFile != NULL;
}

inline uint64 File::GetSizeInBytes()
{
	uint64 FileSize = 0;
	HE_ASSERT( m_pFile != null );

	fseek( m_pFile, 0, SEEK_END );
	FileSize = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_SET );

	return FileSize;
}

inline void File::WriteData(void* pData, uint64 ElementSize, uint64 ElementCount)
{
	HE_ASSERT( m_UsageMode == FUM_Write || m_UsageMode == FUM_Append );
	
	fwrite( pData, ElementSize, ElementCount, GetCFile() );
}

inline void* File::ReadData()
{
	HE_ASSERT( m_UsageMode == FUM_Read );
	
	uint64 FileSizeInBytes = GetSizeInBytes();
	m_pContents = malloc( FileSizeInBytes );
	HE_ASSERT( m_pContents != null );

	fread( m_pContents, 1, FileSizeInBytes, m_pFile );
	HE_ASSERT( m_pContents != null );

	return m_pContents;
}
