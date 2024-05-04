// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

class FPath
{
public:
	FPath( const Char* Str );
	~FPath();

	String ToString() const;

	const String& GetDirectory() const;
	const String& GetFilename() const;
	const String& GetExtention() const;

protected:
	String m_Directory;
	String m_Filename;
	String m_Extention;

};

//
// Inline function implementations
//

inline const String& FPath::ToString() const
{
	return GetDirectory() + GetFilename() + GetExtention();
}

inline const String& FPath::GetDirectory() const
{
	return m_Directory;
}

inline const String& FPath::GetFilename() const
{
	return m_Filename;
}

inline const String& FPath::GetExtention() const
{
	return m_Extention;
}
