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

	bool IsHidden();
	void Show();
	void Hide();

private:
	void RenderWidgets( FCommandContext& CmdContext );


private:
	bool m_IsHidden;
	std::vector<FUIWidget*> m_Widgets;

};

// Inline function implementations
//

FORCEINLINE	bool FUIPanel::IsHidden()
{
	return m_IsHidden;
}

FORCEINLINE void FUIPanel::Show()
{
	m_IsHidden = false;
}

FORCEINLINE void FUIPanel::Hide()
{
	m_IsHidden = true;
}