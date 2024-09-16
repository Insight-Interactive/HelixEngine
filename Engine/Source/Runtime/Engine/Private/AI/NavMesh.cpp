#include "EnginePCH.h"

#include "AI/NavMesh.h"

#include "Engine/GameProject.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"


HNavMesh::HNavMesh() 
	: m_NavMesh( nullptr )
	, m_NavQuery( nullptr )
	, m_npolys(0)
	, m_nstraightPath(0)
	, m_straightPathOptions(0)
{
}

HNavMesh::~HNavMesh() 
{
}

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;
struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};
struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

void HNavMesh::Init()
{
	HE_ASSERT( m_NavMesh == nullptr && m_NavQuery == nullptr );

	FPath Path;
	sprintf_s( Path.m_Path, "%sLevels\\%s", FGameProject::GetInstance()->GetContentFolder(), "DefaultLevel.bin");
	m_NavMesh = LoadNavmesh(Path);
	m_NavQuery = dtAllocNavMeshQuery();
	m_NavQuery->init( m_NavMesh, 2048 );
}

void HNavMesh::UnInit()
{
	dtFreeNavMeshQuery( m_NavQuery );
	m_NavQuery = nullptr;
	dtFreeNavMesh( m_NavMesh );
	m_NavMesh = nullptr;
}

FVector3 HNavMesh::ClosestPointOnNavmesh( const FVector3 Point )
{
	FVector3 Result;
	float Extents[3] = { 100, 100, 100 };
	m_NavQuery->findNearestPoly( &Point.x, Extents, &m_Filter, nullptr, &Result.x);
	return Result;
}

void HNavMesh::FindPath( const FVector3& StartPos, const FVector3& EndPos )
{
	float Extents[3] = { 100, 100, 100 };
	dtPolyRef StartRef, EndRef;
	m_NavQuery->findNearestPoly( &StartPos.x, Extents, &m_Filter, &StartRef, nullptr );
	m_NavQuery->findNearestPoly( &EndPos.x, Extents, &m_Filter, &EndRef, nullptr );
	
	m_NavQuery->findPath( StartRef,EndRef, &StartPos.x, &EndPos.x, &m_Filter, m_Polys, &m_npolys, MAX_POLYS );
	if (m_npolys)
	{
		FVector3 epos = EndPos;
		if (m_Polys[m_npolys - 1] != EndRef)
			m_NavQuery->closestPointOnPoly( m_Polys[m_npolys - 1], &EndPos.x, &epos.x, 0 );

		m_NavQuery->findStraightPath( &StartPos.x, &epos.x, m_Polys, m_npolys,
			m_straightPath, m_straightPathFlags,
			m_straightPathPolys, &m_nstraightPath, MAX_POLYS, m_straightPathOptions );
	}
}

dtNavMesh* HNavMesh::LoadNavmesh( const FPath& Path )
{
	FILE* fp = fopen( Path.m_Path, "rb" );
	if (!fp) return 0;

	// Read header.
	NavMeshSetHeader header;
	size_t readLen = fread( &header, sizeof( NavMeshSetHeader ), 1, fp );
	if (readLen != 1)
	{
		fclose( fp );
		return 0;
	}
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose( fp );
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose( fp );
		return 0;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		fclose( fp );
		return 0;
	}
	dtStatus status = mesh->init( &header.params );
	if (dtStatusFailed( status ))
	{
		fclose( fp );
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		readLen = fread( &tileHeader, sizeof( tileHeader ), 1, fp );
		if (readLen != 1)
		{
			fclose( fp );
			return 0;
		}

		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc( tileHeader.dataSize, DT_ALLOC_PERM );
		if (!data) break;
		memset( data, 0, tileHeader.dataSize );
		readLen = fread( data, tileHeader.dataSize, 1, fp );
		if (readLen != 1)
		{
			dtFree( data );
			fclose( fp );
			return 0;
		}

		mesh->addTile( data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0 );
	}

	fclose( fp );

	return mesh;
}
