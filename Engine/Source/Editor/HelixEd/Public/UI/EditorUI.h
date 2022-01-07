#pragma once


class FViewportContext;
class FCommandContext;
struct ImGuiContext;

class EditorUIContext
{
public:
	EditorUIContext( FViewportContext& Owner );
	~EditorUIContext();
	
	void Setup();
	void Shutdown();
	bool IsValid() const;

	void BeginNewFrame();
	void EndFrame(FCommandContext& CmdContext);

private:
	void SetupImGuiRenderBackend();
private:
	ImGuiContext* m_pImGuiContext;
	FViewportContext& m_Owner;

};
