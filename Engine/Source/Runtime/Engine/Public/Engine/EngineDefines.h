/*
	Defines how the engine should compile.
*/
#ifndef ENGINEDEFINES_H
#define ENGINEDEFINES_H

// ~~~ CAUTION ~~~
// Some projects (below) rely on this files absolute path. Move with caution.
//	- Shaders
//


#define HE_SPLASH_WIDTH		900
#define HE_SPLASH_HEIGHT	450

#if HE_WINDOWS
#	define HE_INPUT_USE_XINPUT	1
#endif


// --------------
// Rendering
// --------------
//
#define HE_MAX_POINT_LIGHTS			4
#define HE_MAX_SPOT_LIGHTS			4
#define HE_MAX_DIRECTIONAL_LIGHTS	4

#define kSceneConstantsReg  b0
#define kMeshWorldReg       b1
#define kMaterialReg        b2
#define kLightsReg          b3

#endif
