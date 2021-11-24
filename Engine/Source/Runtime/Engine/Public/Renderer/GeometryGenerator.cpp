// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/GeometryGenerator.h"
#include "VertexLayouts.h"


namespace GeometryGenerator
{
	StaticMeshGeometryRef GenerateScreenAlignedQuadMesh()
	{
		static const String QuadGeometryName = "ScreenSpaceQuad";
		if (GStaticGeometryManager.MeshExists(QuadGeometryName))
		{
			return GStaticGeometryManager.GetStaticMeshByName(QuadGeometryName);
		}

		uint32 Indices[] =
		{
			0, 1, 3,
			0, 3, 2
		};
		const uint32 NumIndices = HE_ARRAYSIZE(Indices);
		const uint32 IndexBufferSize = NumIndices * sizeof(uint32);

		FVertex2D Verts[] =
		{
			{ { -1.0f, 1.0f }, { 0.0f, 0.0f } }, // Top Left
			{ {  1.0f, 1.0f }, { 1.0f, 0.0f } }, // Top Right
			{ { -1.0f,-1.0f }, { 0.0f, 1.0f } }, // Bottom Left
			{ {  1.0f,-1.0f }, { 1.0f, 1.0f } }, // Bottom Right
		};
		const uint32 NumVerts = HE_ARRAYSIZE(Verts);
		const uint32 VertexBufferSize = NumVerts * sizeof(FVertex2D);

		return GStaticGeometryManager.RegisterGeometry(
			QuadGeometryName,
			Verts, NumVerts, sizeof(FVertex2D),
			Indices, IndexBufferSize, NumIndices
		);
	}

	StaticMeshGeometryRef GenerateSphere(uint32 Radius, uint32 Slices, uint32 Segments)
	{
		Char GeometryInfo[32];
		sprintf_s(GeometryInfo, "Sphere:R%u-S%u-S%u", Radius, Slices, Segments);

		String GeometryName = GeometryInfo;
		if (GStaticGeometryManager.MeshExists(GeometryName))
		{
			return GStaticGeometryManager.GetStaticMeshByName(GeometryName);
		}


		std::vector< FSimpleVertex3D > Verts;
		Verts.resize((Segments + 1) * Slices + 2);

		const float _pi = DirectX::XM_PI;
		const float _2pi = DirectX::XM_2PI;

		Verts[0].Position = FVector3(0, (float)Radius, 0);
		for (int lat = 0; lat < (int)Slices; lat++)
		{
			float a1 = _pi * (float)(lat + 1) / (Slices + 1);
			float sin1 = sinf(a1);
			float cos1 = cosf(a1);

			for (int lon = 0; lon <= (int)Segments; lon++)
			{
				float a2 = _2pi * (float)(lon == Segments ? 0 : lon) / Segments;
				float sin2 = sinf(a2);
				float cos2 = cosf(a2);

				Verts[lon + lat * (Segments + 1) + 1].Position = FVector3(sin1 * cos2 * Radius, cos1 * Radius, sin1 * sin2 * Radius);
			}
		}
		Verts[Verts.size() - 1].Position = FVector3(0, -(float)Radius, 0);

		int NumFaces = (int)Verts.size();
		int NumTris = NumFaces * 2;
		int NumIndices = NumTris * 3;
		std::vector<int>  Triangles(NumIndices);

		int i = 0;
		for (int lon = 0; lon < (int)Segments; lon++)
		{
			Triangles[i++] = lon + 2;
			Triangles[i++] = lon + 1;
			Triangles[i++] = 0;
		}

		// Middle
		for (int lat = 0; lat < (int)Slices - 1; lat++)
		{
			for (int lon = 0; lon < (int)Segments; lon++)
			{
				int current = lon + lat * (Segments + 1) + 1;
				int next = current + Segments + 1;

				Triangles[i++] = current;
				Triangles[i++] = current + 1;
				Triangles[i++] = next + 1;

				Triangles[i++] = current;
				Triangles[i++] = next + 1;
				Triangles[i++] = next;
			}
		}

		// Bottom Cap
		for (int lon = 0; lon < (int)Segments; lon++)
		{
			Triangles[i++] = (int)Verts.size() - 1;
			Triangles[i++] = (int)Verts.size() - (lon + 2) - 1;
			Triangles[i++] = (int)Verts.size() - (lon + 1) - 1;
		}

		int TriCount = (int)Verts.size();
		int IndicesCount = (int)Triangles.size();

		return GStaticGeometryManager.RegisterGeometry(
			GeometryName,
			Verts.data(), TriCount, sizeof(FSimpleVertex3D),
			Triangles.data(), IndicesCount * sizeof(int), IndicesCount
		);
	}
}
