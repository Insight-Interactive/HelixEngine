#pragma once

inline FPath::FPath()
{
	ZeroMemory( m_Path, sizeof( m_Path ) );
}

inline FPath::FPath( const char* Path )
{
	SetPath( Path );
}

inline FPath::~FPath()
{

}

inline FPath::FPath( FPath& other )
{
	memcpy( m_Path, other.m_Path, sizeof( m_Path ) );
}

inline bool FPath::IsValid() const
{
	return m_Path != "\0";
}

inline const char* FPath::GetFullPath() const
{
	return m_Path;
}

inline void FPath::SetPath( const char* Path )
{
	HE_ASSERT( Path != nullptr );

	ZeroMemory( m_Path, sizeof( m_Path ) );

	uint32 PathLength = (uint32)strlen( Path );
	strcpy_s( m_Path, sizeof(m_Path), Path);
}

inline void FPath::Concat( const char* Path )
{
	HE_ASSERT( Path != nullptr );

	strcat_s( m_Path, Path );
}
