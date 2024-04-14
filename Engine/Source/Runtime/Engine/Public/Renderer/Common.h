#pragma once

#include "CommonStructs.h"

// Managers
//
// Manager for all materials used by meshes in the world.
extern class MaterialManager GMaterialManager;
// Manager for all lights in the scene
extern class LightManager GLightManager;

// Input layouts.
extern FInputElementDesc GStaticMeshInputElements[];
extern const uint32 kNumStaticMeshCommonInputElements;

extern FInputElementDesc GSkinnedMeshInputElements[];
extern const uint32 kNumSkinnedMeshCommonInputElements;

extern FInputElementDesc GScreenSpaceInputElements[];
extern const uint32 kNumScreenSpaceInputElements;

extern FInputElementDesc GUITextInputElements[];
extern const uint32 kUITextInputInputElements;

// Sampler Descriptions.
extern FSamplerDesc GLinearWrapSamplerDesc;
