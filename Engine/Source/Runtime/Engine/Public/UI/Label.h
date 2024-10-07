// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "UI/Widget.h"

#include "Graphics/FontManager.h"
#include "Engine/Renderer/Color.h"

const uint32 kMaxTextCharacters = 1024;


class FLabel : public FUIWidget
{
public:
	FLabel();
	~FLabel();

	void SetText( const WChar* Fmt, ... ) 
	{
		ZeroMemory( m_Text, sizeof( m_Text ) );

		va_list args;
		va_start( args, Fmt ); // Start capturing arguments after the 'Fmt' parameter in the method.
		{
			VSPrintBuffer( m_Text, sizeof( m_Text ), Fmt, args );
		}
		va_end( args );
	}
	void SetScale( const FVector2& NewScale ) { m_Scale = NewScale; }
	void SetPadding( const FVector2& NewPadding ) { m_Padding = NewPadding; }
	void SetColor( const FColor& NewColor ) { m_Color = NewColor; }

	const WChar* GetText() const { return m_Text; }
	uint32 GetTextLength() const { return lstrlenW( m_Text ); }
	const FVector2& GetScale() const { return m_Scale; }
	const FVector2& GetPadding() const { return m_Padding; }
	const FColor& GetColor() const { return m_Color; }

protected:
	virtual void Initialize() override;
	virtual void UnInitialize() override;
	
	virtual void Render( FCommandContext& UIContext );

	FDynamicVertexBuffer& GetVertexBuffer();

private:
	void BuildStringVerts();

private:
	WChar m_Text[kMaxTextCharacters];
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
