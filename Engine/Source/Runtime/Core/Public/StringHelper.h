#pragma once

#include "CoreFwd.h"

namespace StringHelper
{
	/*
		Converts a UTF-8 string to a UTF-16 wide string.
	*/
	WString UTF8ToUTF16(const String& str);

	/*
		Converts a UTF-16 wide string to a UTF-8 string.
	*/
	String UTF16ToUTF8(const WString& wStr);

	/*
		Returns the directory from a filepath. Removing the file name.
	*/
	String GetDirectoryFromPath(const String& filepath);

	/*
		Returns the file extension from a file path.
	*/
	String GetFileExtension(const String& filename);

	/*
		Returns the file name from a directory.
	*/
	String GetFilenameFromDirectory(const String& filename);

	/*
		Returns the file name from a directory. Removing the file's extension.
	*/
	String GetFilenameFromDirectoryNoExtension(const String& filename);
}
