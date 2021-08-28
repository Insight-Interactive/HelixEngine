#pragma once

#include "CommonStructs.h"

// Managers
//
// Manager for all materials used by meshes in the world.
extern class MaterialManager GMaterialManager;
// Manager for all lights in the scene
extern class LightManager GLightManager;

// Input layouts.
extern InputElementDesc GSceneMeshInputElements[];
extern const uint32 kNumSceneMeshCommonInputElements;

extern InputElementDesc GScreenSpaceInputElements[];
extern const uint32 kNumScreenSpaceInputElements;

// Sampler Descriptions.
extern SamplerDesc GLinearWrapSamplerDesc;
