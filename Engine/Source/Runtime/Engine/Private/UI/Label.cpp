#include "EnginePCH.h"

#include "UI/Label.h"

#include "AssetRegistry/AssetDatabase.h"


FLabel::FLabel()
	: m_Text( L"<New Label>" )
	, m_Scale(1.f, 1.f)
	, m_Padding(0.f, 0.f)
	, m_Color(1.f, 1.f, 1.f, 1.f)
{
	
}

FLabel::~FLabel()
{

}

void FLabel::Initialize()
{
	FUIWidget::Initialize();

	// TODO: make this resize based on the current sentence size so gpu memory isn't wasted
	m_VertexBuffer.Create( L"Font Verticies", kMaxTextCharacters * sizeof( FTextVertex2D ), sizeof( FTextVertex2D ), nullptr );
	m_Font = FAssetDatabase::GetFont( "Ariel.hfont" );
}

void FLabel::UnInitialize()
{
	FUIWidget::UnInitialize();

}

void FLabel::Render( FCommandContext& UIContext )
{
	FUIWidget::Render( UIContext );

	if (!m_Font.IsValid())
		return;

	UIContext.SetPipelineState( m_Font->GetPipelineState() );
	UIContext.SetGraphicsRootSignature( m_Font->GetRootSignature() );
	UIContext.SetPrimitiveTopologyType( PT_TriangleStrip );
	UIContext.BindVertexBuffer( 0, m_VertexBuffer );
	UIContext.SetTexture( 0, m_Font->GetFontTexture() );

	BuildString();
	UIContext.DrawInstanced( 4, (uint32)m_Text.size(), 0, 0);
}

void FLabel::BuildString()
{
	float topLeftScreenX = (m_Position.x * 2.0f) - 1.0f;
	float topLeftScreenY = ((1.0f - m_Position.y) * 2.0f) - 1.0f;

	float x = topLeftScreenX;
	float y = topLeftScreenY;

	float horrizontalPadding = (m_Font->GetLeftPadding() + m_Font->GetRightPadding()) * m_Padding.x;
	float verticalPadding = (m_Font->GetTopPadding() + m_Font->GetBottomPadding()) * m_Padding.y;

	// cast the gpu virtual address to a textvertex, so we can directly store our vertices there
	FTextVertex2D* vert = (FTextVertex2D*)m_VertexBuffer.GetGpuWritePointer();

	wchar_t lastChar = -1; // no last character to start with

	for (int i = 0; i < m_Text.size(); ++i)
	{
		wchar_t c = m_Text[i];

		const FontCharacter* fc = m_Font->GetCharacter( c );

		// character not in font char set
		if (fc == nullptr)
			continue;

		// end of string
		if (c == L'\0')
			break;

		// new line
		if (c == L'\n')
		{
			x = topLeftScreenX;
			y -= (m_Font->GetLineHeight() + verticalPadding) * m_Scale.y;
			continue;
		}

		// don't overflow the buffer. In your app if this is true, you can implement a resize of your text vertex buffer
		if (i >= kMaxTextCharacters)
			break;

		float kerning = 0.0f;
		if (i > 0)
			kerning = m_Font->GetKerning( lastChar, c );

		vert[i] = FTextVertex2D( m_Color.R,
			m_Color.G,
			m_Color.B,
			m_Color.A,
			fc->U,
			fc->V,
			fc->TextureWidth,
			fc->TextureHeight,
			x + ((fc->XOffset + kerning) * m_Scale.x),
			y - (fc->YOffset * m_Scale.y),
			fc->Width * m_Scale.x,
			fc->Height * m_Scale.y );

		// remove horrizontal padding and advance to next char position
		x += (fc->XAdvance - horrizontalPadding) * m_Scale.x;

		lastChar = c;
	}
}
