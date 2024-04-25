// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


class MeshImporter
{
	friend class AssetImporter;
public:
	enum MeshImportType
	{
		MIT_Invalid,
		MIT_Fbx,
		MIT_Obj,
	};

	void ImportMesh( const Char* Filename, MeshImportType ImportType );
	void ImportMesh( const Char* Filename );
	
protected:
	MeshImporter();
	~MeshImporter();

	void ImportFbx( const Char* Filename );

};

class AssetImporter
{
public:
	AssetImporter();
	~AssetImporter();
	
	void Import( const Char* Filename );

private:
	MeshImporter m_MeshImporter;
};
