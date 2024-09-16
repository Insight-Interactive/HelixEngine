#pragma once


class dtNavMesh;
class dtNavMeshQuery;

class HNavMesh
{
public:
	HNavMesh();
	~HNavMesh();

	void Init();
	void UnInit();

	FVector3 ClosestPointOnNavmesh( const FVector3 Point );
	void FindPath( const FVector3& StartPos, const FVector3& EndPos );

private:
	dtNavMesh* LoadNavmesh( const FPath& Path );

	static const int MAX_POLYS = 256;
	dtPolyRef m_Polys[MAX_POLYS];
	int m_npolys;
	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
	int m_nstraightPath;
	int m_straightPathOptions;

	dtQueryFilter m_Filter;
	dtNavMesh* m_NavMesh;
	dtNavMeshQuery* m_NavQuery;
};