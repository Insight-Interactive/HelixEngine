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
public:
	HStaticMeshGeometry()
	{
	}
	virtual ~HStaticMeshGeometry()
	{
	}

private:

};

