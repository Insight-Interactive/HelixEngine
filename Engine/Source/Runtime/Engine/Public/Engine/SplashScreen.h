#pragma once

#include "Engine/Window.h"

#include "ModelManager.h"
#include "ITextureManager.h"

class ICommandContext;
class IRootSignature;
class IPipelineState;

class SplashScreen : public Window
{
public:
	SplashScreen(const String& SplashTexturePath = "");
	virtual ~SplashScreen();

	void Render( ICommandContext& CmdContext );

	void EndFrame();

protected:
	ViewPort m_ViewPort;
	Rect m_ScissorRect;


	enum
	{
		kSplashTextureRootIndex,
	};

	// Rendering Resources.
	//
	IRootSignature* m_pRootSig;
	IPipelineState* m_pPipeline;
	StaticMeshGeometryRef m_ScreenQuadRef;
	TextureRef m_SplashTexture;
};


// 
// Inline function implementations
//


inline void SplashScreen::EndFrame()
{
	PresentOneFrame();
}