// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "UI/Widget.h"

#include "Renderer/FontManager.h"
#include "Color.h"

const uint32 kMaxTextCharacters = 1024;


class FLabel : public FUIWidget
{
public:
	FLabel();
	~FLabel();

	void SetText( const WString& NewText ) { m_Text = NewText; }
	void SetScale( const FVector2& NewScale ) { m_Scale = NewScale; }
	void SetPadding( const FVector2& NewPadding ) { m_Padding = NewPadding; }
	void SetColor( const FColor& NewColor ) { m_Color = NewColor; }

	const WString& GetText() const { return m_Text; }
	const FVector2& GetScale() const { return m_Scale; }
	const FVector2& GetPadding() const { return m_Padding; }
	const FColor& GetColor() const { return m_Color; }

protected:
	virtual void Initialize() override;
	virtual void UnInitialize() override;
	
	virtual void Render( FCommandContext& UIContext );

	FDynamicVertexBuffer& GetVertexBuffer();

private:
	void DrawLabel();


private:
	WString m_Text;
	HFont m_Font;

	FVector2 m_Scale;
	FVector2 m_Padding;
	FColor m_Color;

	FDynamicVertexBuffer m_VertexBuffer;

};

// Inline function implementations
//

FORCEINLINE FDynamicVertexBuffer& FLabel::GetVertexBuffer()
{
	return m_VertexBuffer;
}
