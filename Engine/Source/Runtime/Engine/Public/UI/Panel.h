#pragma once


class FUIWidget;
class FUIRenderer;
class FColorBuffer;
class FDepthBuffer;
class FViewportContext;
class FCommandContext;
struct FViewPort;
struct FRect;

struct FUIRenderParams
{
	FUIRenderer* pUIRenderer;
	FColorBuffer* pRenderTarget;
	FDepthBuffer* pDepthBuffer;
	FViewPort* pView;
	FRect* pScissor;
	FViewportContext* pRenderingViewport;
};

class FUIPanel
{
	friend class FViewportContext;
	friend class FRenderingSubsystem;
	friend class FUIRenderer;
public:
	FUIPanel();
	~FUIPanel();

	void AddWidget( FUIWidget& Widget );
	bool RemoveWidget( FUIWidget& Widget );

	FUIRenderParams& GetRenderParams() { return m_RenderParams; }
	bool IsDirty() const;
	void WaitForRenderingFinished();

private:
	void RequestRender( FUIRenderParams& RenderParams );
	void OnRenderingFinished();

	void RenderWidgets( FCommandContext& CmdContext );


private:
	FUIRenderParams m_RenderParams;
	bool m_IsDirty;
	std::vector<FUIWidget*> m_Widgets;

};

// Inline function implementations
//

FORCEINLINE	void FUIPanel::RequestRender( FUIRenderParams& RenderParams )
{
	m_RenderParams = RenderParams;
	m_IsDirty = true;
}

FORCEINLINE void FUIPanel::OnRenderingFinished()
{
	m_IsDirty = false;
}

FORCEINLINE bool FUIPanel::IsDirty() const
{
	return m_IsDirty;
}

FORCEINLINE void FUIPanel::WaitForRenderingFinished()
{
	while (IsDirty())
		std::this_thread::yield();
}
