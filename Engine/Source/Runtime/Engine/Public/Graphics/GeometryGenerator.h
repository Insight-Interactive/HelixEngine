// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/ModelManager.h"
#include "Engine/Renderer/RendererCore.h"

namespace GeometryGenerator
{
	/*
		Generates a 2D screen space quad and returns a reference to it.
	*/
	HStaticMesh GenerateScreenAlignedQuadMesh();
	HStaticMesh GenerateSphere(uint32 Radius, uint32 Slices, uint32 Segments);
	HStaticMesh Generate1x1x1CubeMesh();
}
