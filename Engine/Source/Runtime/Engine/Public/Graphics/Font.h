// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"

#include "Engine/Renderer/ManagedAsset.h"

#include "Engine/Renderer/TextureManager.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/RootSignature.h"
#include "Engine/Renderer/PipelineState.h"


struct FontCharacter
{
	WChar Id;

	// These need to be converted to texture coordinates where 0.0 is 0 and 1 is TextureWidth of the font.
	float U;
	float V;
	float TextureWidth; // Width of the character on texture
	float TextureHeight; // Height of the character on texture

	float Width; // The width of the character in screen coords
	float Height; // The height of the character in screen coords

	// These need to be normalized  based on the font size.
	float XOffset; // Offset of the current character pos to the left side of the character
	float YOffset; // Offset from the top line to top of character
	float XAdvance; // How far to move to the rightfor the next character
};

struct FontKerning
{
	WChar FirstId; // The first character
	WChar SecondId; // The second character
	float Amount; // The amount to add/subtract to second character/s X
};

class FFont : public ManagedAsset<FFont>
{
	friend class FFontManager;
	friend class FUIRenderer;
public:
	FFont();
	~FFont();

	float GetKerning( WChar First, WChar Second );
	const FontCharacter* GetCharacter( WChar Character );

	HTexture GetFontTexture() { return m_FontTexture; }

	float GetLeftPadding() { return m_LeftPadding; }
	float GetRightPadding() { return m_RightPadding; }
	float GetTopPadding() { return m_TopPadding; }
	float GetBottomPadding() { return m_BottomPadding; }

	float GetLineHeight() { return m_LineHeight; }
	uint32 GetNumCharacters() {return m_NumCharacters;}

	FPipelineState& GetPipelineState() { return m_Pipeline; }
	FRootSignature& GetRootSignature() { return m_RootSig; }
	
protected:
	void Initialize( const String& Filename );
	void LoadFont( const String& filename, int windowWidth, int windowHeight );

	void SetNumCharacters( uint32 NumCharacters ) { m_NumCharacters = NumCharacters; }

protected:
	HTexture m_FontTexture;
	FRootSignature m_RootSig;
	FPipelineState m_Pipeline;


	WString m_FontName; // TODO: Is this needed?
	WString m_FontImage; // TODO: Is this needed?

	uint32 m_Size; // Size of the font. m_LineHeight and m_BaseHeight will be based on this as if this is a single unit (1.0)
	float m_LineHeight; // For far down to move to next line, will be normalized.
	float m_BaseHeight; // Height of all characters, will be normalized.
	std::vector<FontCharacter> m_Characters;
	std::vector<FontKerning> m_Kernings;

	float m_LeftPadding;
	float m_TopPadding;
	float m_RightPadding;
	float m_BottomPadding;

	uint32 m_NumCharacters;
};

typedef ManagedAsset<FFont> ManagedFont;
typedef AssetRef<FFont> HFont;
