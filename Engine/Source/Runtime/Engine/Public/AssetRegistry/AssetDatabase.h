#pragma once

#include "TSingleton.h"

#include "AssetRegistry/MeshDatabase.h"


class AssetDatabase : public TSingleton<AssetDatabase>
{
public:
	AssetDatabase();
	virtual ~AssetDatabase();

	void Initialize( const Char* DatabaseFile );
	void UnInitialize();

	void RegisterMesh( const Char* MeshName, const Char* Filepath );

protected:
	MeshDatabase& GetMeshDatabase();

protected:
	MeshDatabase m_MeshDatabase;

};

//
// Inline function implementations
//

inline MeshDatabase& AssetDatabase::GetMeshDatabase()
{
	return m_MeshDatabase;
}

inline void AssetDatabase::RegisterMesh( const Char* MeshName, const Char* Filepath )
{
	m_MeshDatabase.RegisterMesh( MeshName, Filepath );
}
