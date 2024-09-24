#pragma once

#include "DebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"


class HNavMesh
{
public:
	HNavMesh();
	~HNavMesh();

	void Init(const FPath& Path);
	void UnInit();

	FVector3 ClosestPointOnNavmesh( const FVector3 Point );
	void FindPath( const FVector3& StartPos, const FVector3& EndPos );

	static const int MAX_POLYS = 256;
	float m_straightPath[MAX_POLYS * 3];
	int m_nstraightPath;
private:
	dtNavMesh* LoadNavmesh( const FPath& Path );

	dtPolyRef m_Polys[MAX_POLYS];
	int m_npolys;
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
	int m_straightPathOptions;

	dtQueryFilter m_Filter;
	dtNavMesh* m_NavMesh;
	dtNavMeshQuery* m_NavQuery;
};

class NavMeshDebugDraw : public duDebugDraw
{
public:
	virtual void depthMask( bool state );
	virtual void texture( bool state );
	virtual void begin( duDebugDrawPrimitives prim, float size = 1.0f );
	virtual void vertex( const float* pos, unsigned int color );
	virtual void vertex( const float x, const float y, const float z, unsigned int color );
	virtual void vertex( const float* pos, unsigned int color, const float* uv );
	virtual void vertex( const float x, const float y, const float z, unsigned int color, const float u, const float v );
	virtual void end();
};
