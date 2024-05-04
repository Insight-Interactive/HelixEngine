// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "ModelManager.h"
#include "RendererCore.h"

namespace GeometryGenerator
{
	/*
		Generates a 2D screen space quad and returns a reference to it.
	*/
	StaticMeshGeometryRef GenerateScreenAlignedQuadMesh();
	StaticMeshGeometryRef GenerateSphere(uint32 Radius, uint32 Slices, uint32 Segments);
	StaticMeshGeometryRef Generate1x1x1CubeMesh();
}
