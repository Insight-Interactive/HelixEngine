// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/HLevel.h"

#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Actor/APlayerCharacter.h"
#include "RendererCore.h"
#include "ModelManager.h"
#include "World/HWorld.h"

#include "GameFramework/Actor/Components/HStaticMeshComponent.h"

#include "RapidXML/rapidxml.hpp"


HLevel::HLevel(HWorld* pOwner)
	: m_pOwner(pOwner)
{
}

HLevel::~HLevel()
{
}

void HLevel::LoadFromFile(const Char* FileName)
{
	{
		/////////////////////////
		// Model Asset Design
		// All models are imported and turned into this custom format. (.fbx/.obj -> .hasset)
		// 
		// Header:
		//		Num Mappings	( 4 Bytes )
		//		Mappings Size	( 4 Bytes )
		// 
		// Mesh Mappings:
		//		MeshName Hash ( 4 Bytes ) -> Offset in bytes from file start ( 4 Bytes )
		//		... n meshes
		// 
		// Mesh Data:
		//		Mesh Subheader:
		//		Num Verticies	( 4 Bytes )
		//		Vertex Size		( 4 Bytes )
		//		Num Indicies	( 4 Bytes )
		//		Index Size		( 4 Bytes )
		//		Mesh1 Vertex 1
		//		... Mesh1 n verticies
		//		
		/////////////////////////


		/////////////////////////
		//	Imaginary File
		//	File Path - Content/Models/Tank.fbx
		//	Filename - Tank.fbx
		//	Geometry - Turret
		//	Geometry - Body
		//	Geometry - Track Left
		//	Geometry - Track Right
		/////////////////////////
		
		/////////////////////////
		//	Imaginary Actor
		//	Name - Destroyed Tank
		//	Component - Static Mesh
		//	Geometry - Body
		/////////////////////////


		// Mesh Database
		// ( Simple map - Contains all mesh assets for the project )
		// Turret		-> Content/Models/Tank.hasset
		// Body			-> Content/Models/Tank.hasset
		// Track Left	-> Content/Models/Tank.hasset
		// Track Right	-> Content/Models/Tank.hasset

		/*
		
		StaticMeshGeometryRef GetStaticMeshGeometryByName(const char* Name)
		{
			if(m_ActiveModelCache[Name].exists()) 
				return m_ActiveModelCache[Name];

			const char* FilePath = MeshDataBase[Name];

			HAsset ModelAsset = LoadAsset(FilePath);
			int NameHash = StringHash(Name, strlen(Name));

			uint32 NumMappings = ModelAsset.Data();
			uint32 MappingSize = ModelAsset.Data() + sizeof(uint32)
			void* DataHead = ModelAsset.Data();

			DataHead += 8; // Move past header to mappings.

			uint32 MeshOffset = 0;
			for(uint32 i = 0; i < NumMappings; ++i)
			{
				if( *DataHead == NameHash)
				{
					// Found the mesh.
					DataHead += 4; // Move to Mesh offset from file start.
					MeshOffset = *DataHead;
					break;
				}
				else
				{
					DataHead += 8; // Move to the next row.
				}
			}

			void* MeshRawData = ModelAsset.Data() + MeshOffset;
			// Parse the submesh data to find out how many verticies we need for this mesh.
			uint32 NumVerticies = *MeshRawData;
				MesRawData += sizeof(uint32);
			uint32 VertexSize = *RawMeshData;
				MesRawData += sizeof(uint32); 
			uint32 NumIndices = *MeshRawData;
				MesRawData += sizeof(uint32); 
			uint32 IndexSize = *MeshRawData;
				MesRawData += sizeof(uint32); // Now pointing to first vertex.

			uint32 MeshSizeInBytes = NumVerticies * VertexSize;
			Vertex3D* pMeshVerticies = ( Vertex3D* )malloc( MeshSizeInBytes ); // Allocate memory to hold all of the verticies.
			memcpy(pMeshVerticies, MeshRawData, MeshSizeInBytes); // could we just use "MeshRawData" immediatly instead of stacing the buffer on the cpu again?
			
			MesRawData += MeshSizeInBytes; // Now pointing to the first index.

			uint32 MeshIndicesSizeInBytes = NumIndices * IndexSize;
			uint32* pMeshIndexData = (uint32*) malloc( MeshIndicesSizeInBytes );

			// Upload and register with the gpu.
			ManagedStaticMeshGeometry* pMesh = NULL;
			pMesh = new ManagedStaticMeshGeometry(Name);
			pMesh->SetHashName(HashName);
			pMesh->Create(pMeshVerticies, MeshSizeInBytes, NumVerticies, VertexSize, pMeshIndexData, MeshIndicesSizeInBytes, NumIndices);
			pMesh->SetLoadCompleted(true);

			m_ActiveModelCache[Name].reset(pMesh);

			return m_ActiveModelCache[Name];
		}
		
		*/


		// Mesh database
		// Submesh name -> Owning file filename
		// Submesh name -> Owning file filename
		// ...

		// Mesh Manager
		/*
			[in] SubmeshName

			Filename = MeshDatabase[Submesh name]

			MeshFile = LoadMesh(Filename);
			Foreach submesh in MeshFile
				if submesh.name == SubmeshName
					ParseMeshfile and return MeshRef
		*/



	}

	// Load Resources
	//
	StaticMeshGeometryRef CubeMesh = GStaticGeometryManager.LoadHAssetMeshFromFile("Content/Models/SM_Cube.hasset");
	GMaterialManager.LoadMaterialFromFile("M_RustedMetal.hmat");
	TextureRef AlbedoTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Albedo.dds", DT_Magenta2D, false);
	TextureRef NormalTexture = GTextureManager->LoadTexture("Content/Textures/RustedIron/RustedIron_Normal.dds", DT_Magenta2D, false);
	
	// Get References
	//
	MaterialRef RustedMetalMaterial = GMaterialManager.GetMaterialByName("M_RustedMetal");
	
	RustedMetalMaterial->SetAlbedoTexture(AlbedoTexture);
	RustedMetalMaterial->SetNormalTexture(NormalTexture);

	AActor* CubeActor = CreateActor<AActor>();
	HStaticMeshComponent* SMSubeActorCube = CubeActor->AddComponent<HStaticMeshComponent>(TEXT("Cube Mesh"));
	SMSubeActorCube->SetMaterial(RustedMetalMaterial);
	SMSubeActorCube->SetMesh(CubeMesh);

	APlayerCharacter* pPlayer = CreateActor<APlayerCharacter>();
	m_pOwner->SetCurrentSceneRenderCamera(pPlayer->GetCameraComponent());

	return;
	FILE* pFile;
	fopen_s( &pFile, "Content/Actors/ThirdPersonPlayerCharacter.xml", "r" );
	char* Buffer = null;

	fseek( pFile, 0, SEEK_END );
	long FileSize = ftell( pFile );
	rewind( pFile );

	Buffer = (char*)HE_HeapAlloc( FileSize );
	ZeroMemory( Buffer, FileSize );
	fread( Buffer, 1, FileSize, pFile );

	rapidxml::xml_document<char> doc;
	doc.parse<0>( Buffer );

	enum AssetType
	{
		Actor,
		Material,
	};

	enum ActorCategory
	{
		UserDefined,
		Base,
		PlayerCharacter,
		PlayerController,
		Pawn,
	};

	rapidxml::xml_node<>* pAssetNode = doc.first_node( "Asset" );
	char* val = pAssetNode->value();
	// Type
	rapidxml::xml_attribute<char>* pType = pAssetNode->first_attribute();
	char* TypeName = pType->name();
	char* TypeValue = pType->value();
	AssetType AssetCategory = (AssetType)(TypeValue[0] - 48);
	// Category
	rapidxml::xml_attribute<char>* pCategory = pType->next_attribute();
	char* pCategoryName = pCategory->name();
	char* pCategoryValue = pCategory->value();
	ActorCategory ActorCat = (ActorCategory)(pCategoryValue[0] - 48);

	rapidxml::xml_node<>* pDisplayNameNode = pAssetNode->first_node( "DisplayName" );
	char* DisplayName = pDisplayNameNode->value();

	rapidxml::xml_node<>* pComponentsNode = pDisplayNameNode->next_sibling();
	rapidxml::xml_node<>* pSMNode = pComponentsNode->first_node( "StaticMeshComponent" );
	rapidxml::xml_attribute<char>* pSMFileInfo = pSMNode->first_attribute();
	char* SMFileName = pSMFileInfo->name();
	char* SMFilepath = pSMFileInfo->value();

	AActor* pNewActor = CreateActor<AActor>();
	

	HE_HeapFree( Buffer );
	fclose( pFile );
}

void HLevel::BeginPlay()
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->BeginPlay();
	}
}

void HLevel::Tick(float DeltaTime)
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Tick(DeltaTime);
	}
}

void HLevel::Flush()
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->RemoveAllComponents();
		delete m_Actors[i];
	}
}

void HLevel::Render(ICommandContext& CmdContext)
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Render(CmdContext);
	}
}
