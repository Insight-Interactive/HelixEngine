// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


// Safely release a PhysX API object.
#define PX_SAFE_RELEASE( PXObject ) if( (PXObject) != nullptr ) { (PXObject)->release(); (PXObject) = nullptr; } 

#define P_MAX_NUM_ACTOR_SHAPES 128
