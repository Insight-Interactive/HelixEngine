// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "MeshGeometry.h"


/*
	Base class for all static geometry that exists in the world. That is, a peice of 
	geometry with static unskinned polygons.
*/
class RENDER_API HStaticMeshGeometry : public FMeshGeometry
{
	friend class FStaticGeometryManager;
	friend class FAssetDatabase;
public:
	HStaticMeshGeometry()
	{
	}
	virtual ~HStaticMeshGeometry()
	{
	}

};


