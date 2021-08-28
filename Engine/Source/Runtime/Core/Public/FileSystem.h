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
