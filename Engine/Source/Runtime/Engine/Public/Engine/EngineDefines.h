// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
	Defines how the engine should compile.
*/
#ifndef ENGINEDEFINES_H
#define ENGINEDEFINES_H

// ~~~ CAUTION ~~~
// Some projects (below) rely on this files absolute path. Move with caution.
//	- Shaders
//


// ----------------------------
//			Input
// ----------------------------

#define HE_INPUT_USE_KEYBOARD_MOUSE 1
#define HE_INPUT_USE_GAMEPAD		1

#if HE_WINDOWS && HE_INPUT_USE_GAMEPAD
//	If using Xinput for game controllers.
#	define HE_INPUT_USE_XINPUT	1
#endif


// ----------------------------
//			Splash
// ----------------------------

#define HE_SPLASH_WIDTH		900
#define HE_SPLASH_HEIGHT	450

#if HE_GAME_CONSOLE
#	define HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH 0
#else
#	define HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH 1
#endif


// ----------------------------
//			Rendering
// ----------------------------

// General
#define HE_MAX_SCENE_DEPTH			1
// Lighting
#define HE_MAX_POINT_LIGHTS			4
#define HE_MAX_SPOT_LIGHTS			4
#define HE_MAX_DIRECTIONAL_LIGHTS	4
// Shader constant buffer registers
// These register indicies are reserved and constant across all shaders and must NOT be changed!
// New constant buffers should reside in indicies immediatly following the last index to insude no buffer registers are duplicated.
#define kSceneConstantsReg  0
#define kMeshWorldReg       1
#define kLightsReg          2

// Simple redefines of symbols so they can match in the C++ code and in the shaders.
// Shaders just get the value of the macro, C++ just stringifies the macro.
// Constant buffers
#define SceneConstants_CB	SceneConstants_CB
#define MeshWorld_CB		MeshWorld_CB
#define SceneLights_CB		SceneLights_CB
// Samplers
#define LinearWrapSampler	LinearWrapSampler

#endif // ENGINEDEFINES_H
