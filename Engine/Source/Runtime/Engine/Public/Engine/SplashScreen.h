#pragma once

#include "Engine/Window.h"

#include "ModelManager.h"
#include "TextureManager.h"
#include "RootSignature.h"
#include "PipelineState.h"


class FCommandContext;
class FRootSignature;
class FPipelineState;

class FSplashScreen : public FWindow
{
public:
	FSplashScreen(const String& SplashTexturePath = "");
	virtual ~FSplashScreen();

	void Render( FCommandContext& CmdContext );

	void EndFrame();

	FViewPort& GetViewport();
	FRect& GetScissorRect();

protected:
	FViewPort m_ViewPort;
	FRect m_ScissorRect;


	enum
	{
		kSplashTextureRootIndex,
	};

	// Rendering Resources.
	//
	FRootSignature m_RootSig;
	FPipelineState m_Pipeline;
	StaticMeshGeometryRef m_ScreenQuadRef;
	HTexture m_SplashTexture;
};


// 
// Inline function implementations
//


inline void FSplashScreen::EndFrame()
{
	PresentOneFrame();
}

inline FViewPort& FSplashScreen::GetViewport()
{
	return m_ViewPort;
}

inline FRect& FSplashScreen::GetScissorRect()
{
	return m_ScissorRect;
}
