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


// ---------
// Input
// ---------
#define HE_INPUT_USE_KEYBOARD_MOUSE 1
#define HE_INPUT_USE_GAMEPAD		1

#if HE_WINDOWS && HE_INPUT_USE_GAMEPAD
//	If using Xinput for game controllers.
#	define HE_INPUT_USE_XINPUT	1
#endif


// ---------
// Splash
// ---------

#define HE_SPLASH_WIDTH		900
#define HE_SPLASH_HEIGHT	450

#if HE_GAME_CONSOLE
#	define HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH 0
#else
#	define HE_PLATFORM_USES_WHOLE_WINDOW_SPLASH 1
#endif


// --------------
// Rendering
// --------------
//
// General
#define kMaxSceneDepth				1
// Lighting
#define HE_MAX_POINT_LIGHTS			4
#define HE_MAX_SPOT_LIGHTS			4
#define HE_MAX_DIRECTIONAL_LIGHTS	4
// Shader constant buffer registers
#define kSceneConstantsReg  0
#define kMeshWorldReg       1
#define kMaterialReg        2
#define kLightsReg          3

#endif
