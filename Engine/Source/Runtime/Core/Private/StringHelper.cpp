#include "CorePCH.h"

#include "StringHelper.h"


namespace StringHelper
{
	WString UTF8ToUTF16(const String& str)
	{
		return WString(str.begin(), str.end());
	}

	String UTF16ToUTF8(const WString& wStr)
	{
		HE_PRAGMA_DISABLE
		(
			4244,
			return String(wStr.begin(), wStr.end());
		);
	}

	String GetDirectoryFromPath(const String& filepath)
	{
		size_t off1 = filepath.find_last_of(HE_TEXT('\\'));
		size_t off2 = filepath.find_last_of(TEXT('/'));
		if (off1 == String::npos && off2 == String::npos) //If no slash or backslash in path?
		{
			return { };
		}
		if (off1 == String::npos)
		{
			return filepath.substr(0, off2);
		}
		if (off2 == String::npos)
		{
			return filepath.substr(0, off1);
		}
		//If both exists, need to use the greater offset
		//return filepath.substr(0, std::max(off1, off2));
		return String{};
	}

	String GetFileExtension(const String& Filename)
	{
		size_t Offset = Filename.find_last_of(TEXT('.'));
		if (Offset == String::npos)
		{
			return String{};
		}
		return String(Filename.substr(Offset + 1));
	}

	String GetFilenameFromDirectory(const String& filename)
	{
		String result = filename;

		// Erase everything up to the beginning of the filename
		const size_t last_slash_idx = result.find_last_of("\\/");
		if (String::npos != last_slash_idx)
			result.erase(0, last_slash_idx + 1);

		return result;
	}

	String GetFilenameFromDirectoryNoExtension(const String& filename)
	{
		String result = filename;

		// Erase everything up to the beginning of the filename
		const size_t last_slash_idx = result.find_last_of("\\/");
		if (String::npos != last_slash_idx)
			result.erase(0, last_slash_idx + 1);

		// Erase the file extension
		const size_t period_idx = result.rfind('.');
		if (String::npos != period_idx)
			result.erase(period_idx);

		return result;
	}
}
