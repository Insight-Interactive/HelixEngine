#include "HelixEdPCH.h"

#include "Tools/AssetImporter.h"

#include "FileSystem.h"
#include "StringHelper.h"
#include "VertexLayouts.h"
#include "AssetRegistry/Asset.h"
#include "AssetRegistry/AssetDatabase.h"

#include "miniz.c"
#include "ofbx.h"
#include "ofbx.cpp"


//
// Asset Importer
//


AssetImporter::AssetImporter()
{

}

AssetImporter::~AssetImporter()
{

}

void AssetImporter::Import( const Char* Filename )
{
	String FileExtension = StringHelper::GetFileExtension( Filename );

	if (FileExtension == "fbx")
	{
		m_MeshImporter.ImportMesh( Filename, MeshImporter::MIT_Fbx );
	}
}

//
// Mesh Importer
//


MeshImporter::MeshImporter()
{

}

MeshImporter::~MeshImporter()
{

}

void MeshImporter::ImportMesh( const Char* Filename, MeshImportType ImportType )
{
	HE_ASSERT( ImportType != MIT_Invalid );
	HE_LOG( Log, TEXT( "Importing mesh with path: \"%s\"" ), CharToTChar( Filename ) );

	switch (ImportType)
	{
	case MIT_Fbx:
		ImportFbx( Filename );
		break;
	}
}

void MeshImporter::ImportMesh( const Char* Filename )
{
	String FileExtension = StringHelper::GetFileExtension( Filename );

	MeshImportType ImportType = MIT_Invalid;
	if (FileExtension == "fbx")
	{
		ImportType = MIT_Fbx;
	}

	ImportMesh( Filename, ImportType );
}

void MeshImporter::ImportFbx( const Char* Filename )
{
	// Read the data file from disk.
	//
	DataBlob FileData = FileSystem::ReadRawData( Filename );

	const ofbx::u8* pData = (const ofbx::u8*)FileData.GetBufferPointer();
	std::vector<FVertex3D> Verticies;
	std::vector<uint32> Indices;

	// Process the mesh as an fbx file.
	//
	ofbx::IScene* pScene = ofbx::load(
		pData,
		(int)FileData.GetDataSize(),
		(ofbx::u64)ofbx::LoadFlags::TRIANGULATE
	);
	if (pScene == NULL)
	{
		HE_LOG( Error, TEXT( "An error occured when importing model! Error code: %s" ), ofbx::getError() );
		return;
	}

	// Parse the mesh.
	//
	const ofbx::GlobalSettings* s = pScene->getGlobalSettings();
	for (int i = 0; i < pScene->getMeshCount(); ++i)
	{
		// Read th FBX file data.
		//
		const ofbx::Mesh& Mesh = *pScene->getMesh( i );
		std::vector<FVertex3D> Verticies; Verticies.reserve( Mesh.getGeometry()->getVertexCount() );
		std::vector<uint32> Indices; Indices.reserve( Mesh.getGeometry()->getIndexCount() );

		String MeshName = StringHelper::GetFilenameFromDirectory( Mesh.name );

		const ofbx::Geometry& Geometry = *(Mesh.getGeometry());

		const ofbx::Vec3* RawVerticies = Geometry.getVertices();
		int VertexCount = Geometry.getVertexCount();

		for (int i = 0; i < VertexCount; ++i)
		{
			FVertex3D Vertex;

			Vertex.Position.x = (float)RawVerticies[i].x;
			Vertex.Position.y = (float)RawVerticies[i].y;
			Vertex.Position.z = (float)RawVerticies[i].z;

			if (Geometry.getNormals() != nullptr)
			{
				const ofbx::Vec3* normals = Geometry.getNormals();

				Vertex.Normal.x = (float)normals[i].x;
				Vertex.Normal.y = (float)normals[i].y;
				Vertex.Normal.z = (float)normals[i].z;
			}

			if (Geometry.getUVs() != nullptr)
			{
				const ofbx::Vec2* uvs = Geometry.getUVs();
				int count = Geometry.getIndexCount();

				Vertex.UV0.x = (float)uvs[i].x;
				Vertex.UV0.y = (float)uvs[i].y;

				const ofbx::Vec3* tans = Geometry.getTangents();
				if (tans)
				{
					Vertex.Tangent.x = (float)tans[i].x;
					Vertex.Tangent.y = (float)tans[i].y;
					Vertex.Tangent.z = (float)tans[i].z;

					FVector3 TempTangent( (float)tans[i].x, (float)tans[i].y, (float)tans[i].z );
					FVector3 Normal( Vertex.Normal.x, Vertex.Normal.y, Vertex.Normal.z );
					FVector3 BiTangent = Normal.Cross( TempTangent );

					Vertex.BiTangent.x = BiTangent.x;
					Vertex.BiTangent.y = BiTangent.y;
					Vertex.BiTangent.z = BiTangent.z;
				}
				else
				{
					Vertex.UV0 = FVector2( 0.0f, 0.0f );
					Vertex.Tangent = FVector3( 0.0f, 0.0f, 0.0f );
					Vertex.BiTangent = FVector3( 0.0f, 0.0f, 0.0f );
				}

				if (Geometry.getColors() != NULL)
				{
					const ofbx::Vec4* Colors = Geometry.getColors();
					Vertex.Color.x = (float)Colors[0].x;
					Vertex.Color.y = (float)Colors[0].y;
					Vertex.Color.z = (float)Colors[0].z;
					Vertex.Color.w = (float)Colors[0].w;
				}
			}

			Verticies.push_back( Vertex );
		}

		const int* RawFaceIndicies = Geometry.getFaceIndices();
		int IndexCount = Geometry.getIndexCount();
		for (int i = 0; i < IndexCount; ++i)
		{
			int idx = RawFaceIndicies[i];

			// If the index is negative in fbx that means it is the last index of that polygon.
			// So, make it positive and subtract one.
			if (idx < 0)
				idx = -(idx + 1);

			Indices.push_back( idx );
		}

		// Write the fbx data to out to custom .hasset file.
		//
		HMeshAsset::HMeshAssetHeader Header;
		Header.NumVerticies = (uint32)Verticies.size();
		Header.VertexSizeInBytes = sizeof( FVertex3D );
		Header.NumIndices = (uint32)Indices.size();
		Header.IndexSizeInBytes = sizeof( uint32 );

		String OutputFile = FGameProject::GetInstance()->GetContentFolder();
		OutputFile.append( "/" );
		OutputFile.append( MeshName );
		OutputFile.append( ".hasset" );
		FileRef Output( OutputFile.c_str(), FUM_Write, CM_Binary );

		if (Output->IsOpen())
		{
			Output->WriteData( &Header, sizeof( HMeshAsset::HMeshAssetHeader ), 1 );
			Output->WriteData( Verticies.data(), sizeof( FVertex3D ), Verticies.size() );
			Output->WriteData( Indices.data(), sizeof( uint32 ), Indices.size() );

			FGUID Guid = FGUID::Generate();
			FAssetDatabase::GetInstance()->RegisterMesh( Guid, OutputFile.c_str() );

			HE_LOG( Log, TEXT( "Mesh import with path: \"%s\" successful." ), CharToTChar( Filename ) );
		}
		else
		{
			HE_LOG( Error, TEXT( "Failed to open file with path: \"%s\" for importing." ), CharToTChar( OutputFile ) );
			HE_ASSERT( false );
		}
	}
}
