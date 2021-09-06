#pragma once

#include "StringHelper.h"


class MeshDatabase
{
public:

	void Initialize( const Char* MeshDatabaseFile );
	void UnInitialize();

	void RegisterMesh( const char* MeshName, const char* Filepath );
	void UnRegisterMesh( const char* MeshName );

protected:
	/*
		Mapping of all meshes this project prosesses.
	*/
	std::map<String, String> m_ProjectMeshes;

};

//
// Inline funciton implementations
//

inline void MeshDatabase::RegisterMesh( const char* MeshName, const char* Filepath )
{
	HE_LOG( Log, TEXT( "Registering mesh \"%s\" into database." ), CharToTChar( MeshName ) );
	m_ProjectMeshes.try_emplace( String(MeshName), String(Filepath) );
}

inline void MeshDatabase::UnRegisterMesh( const char* MeshName )
{
	auto Iter = m_ProjectMeshes.find( MeshName );
	if (Iter != m_ProjectMeshes.end())
	{
		m_ProjectMeshes.erase( Iter );
	}
	else
	{
		HE_LOG( Warning, TEXT( "Trying to unregister a mesh (\"%s\") from database that does not exist!" ), CharToTChar( MeshName ) );
		HE_ASSERT( false );
	}
}
