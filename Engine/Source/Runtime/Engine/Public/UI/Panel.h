#pragma once


class FUIWidget;
class FUIRenderer;
class FColorBuffer;
class FDepthBuffer;
class FViewportContext;
class FCommandContext;
struct FViewPort;
struct FRect;

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

private:
	void OnRenderingFinished();

	void RenderWidgets( FCommandContext& CmdContext );


private:
	std::vector<FUIWidget*> m_Widgets;

};

// Inline function implementations
//
