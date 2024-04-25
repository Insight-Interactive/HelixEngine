#include "EnginePCH.h"

#include "Renderer/Font.h"

#include "FileSystem.h"
#include "AssetRegistry/AssetDatabase.h"
#include "CommonStructHelpers.h"

FFont::FFont()
    : m_NumCharacters(0)
{

}

FFont::~FFont()
{

}

void FFont::Initialize( const String& Filename )
{
	DataBlob VSShader = FileSystem::ReadRawData( FAssetDatabase::LookupShaderPath( FGUID::CreateFromString( "41c75002-f4b1-4030-adec-7a5088a4e65b" ) ).c_str() );
	DataBlob PSShader = FileSystem::ReadRawData( FAssetDatabase::LookupShaderPath( FGUID::CreateFromString( "c7f15311-d769-4753-b311-db6a33c2b9b0" ) ).c_str() );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GUITextInputElements;
	PSODesc.InputLayout.NumElements = kUITextInputInputElements;
	
	m_RootSig.Reset( 1, 1 );
	m_RootSig.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Font texture
	m_RootSig[0].InitAsDescriptorTable( 1, SV_Pixel );
	m_RootSig[0].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	m_RootSig.Finalize( L"[UI Renderer] Font Root Signature", RSF_AllowInputAssemblerLayout );
	PSODesc.pRootSignature = &m_RootSig;

	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.RTVFormats[0] = F_R8G8B8A8_UNorm; // TODO: Should be the resolution of the render target.
	PSODesc.SampleDesc = { 1, 0 };;
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.RasterizerDesc = CRasterizerDesc();

	FBlendDesc BlendDesc = CBlendDesc();
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	BlendDesc.RenderTarget[0].SourceBlend = B_SourceAlpha;
	BlendDesc.RenderTarget[0].DestBlend = B_One;
	BlendDesc.RenderTarget[0].BlendOp = BO_Add;
	BlendDesc.RenderTarget[0].SourceBlendAlpha = B_SourceAlpha;
	BlendDesc.RenderTarget[0].DestBlendAlpha = B_One;
	BlendDesc.RenderTarget[0].BlendOpAlpha = BO_Add;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	PSODesc.BlendState = BlendDesc;
	PSODesc.NumRenderTargets = 1;

	CDepthStencilStateDesc DepthStateDesc;
	DepthStateDesc.DepthEnable = false;
	PSODesc.DepthStencilState = DepthStateDesc;
	m_Pipeline.Initialize( PSODesc );
    m_FontTexture = FAssetDatabase::GetTexture( FGUID::CreateFromString( "6878e921-457d-47f8-8307-2ce37456e019" ) );
    LoadFont( Filename, 1920, 1080 );// TODO: This need to be the viewport dimensions
}

void FFont::LoadFont( const String& filename, int windowWidth, int windowHeight )
{
    HE_ASSERT( m_FontTexture.IsValid() );

    std::wifstream fs;
    fs.open( filename );

    std::wstring tmp;
    size_t startpos;

    // extract font name
    fs >> tmp >> tmp; // info face="Arial"
    startpos = tmp.find( L"") + 1;
    m_FontName = tmp.substr( startpos, tmp.size() - startpos - 1 );

    // get font size
    fs >> tmp; // size=73
    startpos = tmp.find( L"=" ) + 1;
    m_Size = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

    // bold, italic, charset, unicode, stretchH, smooth, aa, padding, spacing
    fs >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 

    // get padding
    fs >> tmp; // padding=5,5,5,5 
    startpos = tmp.find( L"=" ) + 1;
    tmp = tmp.substr( startpos, tmp.size() - startpos ); // 5,5,5,5

    // get up padding
    startpos = tmp.find( L"," ) + 1;
    m_TopPadding = std::stoi( tmp.substr( 0, startpos ) ) / (float)windowWidth;

    // get right padding
    tmp = tmp.substr( startpos, tmp.size() - startpos );
    startpos = tmp.find( L"," ) + 1;
    m_RightPadding = std::stoi( tmp.substr( 0, startpos ) ) / (float)windowWidth;

    // get down padding
    tmp = tmp.substr( startpos, tmp.size() - startpos );
    startpos = tmp.find( L"," ) + 1;
    m_BottomPadding = std::stoi( tmp.substr( 0, startpos ) ) / (float)windowWidth;

    // get left padding
    tmp = tmp.substr( startpos, tmp.size() - startpos );
    m_LeftPadding = std::stoi( tmp ) / (float)windowWidth;

    fs >> tmp; // spacing=0,0

    // get lineheight (how much to move down for each line), and normalize (between 0.0 and 1.0 based on size of font)
    fs >> tmp >> tmp; // common lineHeight=95
    startpos = tmp.find( L"=" ) + 1;
    m_LineHeight = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)windowHeight;

    // get base height (height of all characters), and normalize (between 0.0 and 1.0 based on size of font)
    fs >> tmp; // base=68
    startpos = tmp.find( L"=" ) + 1;
    m_BaseHeight = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)windowHeight;

    // get texture width
    fs >> tmp; // scaleW=512
    startpos = tmp.find( L"=" ) + 1;
    uint32 TextureWidth = (uint32)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

    // get texture height
    fs >> tmp; // scaleH=512
    startpos = tmp.find( L"=" ) + 1;
    uint32 TextureHeight = (uint32)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

    // get pages, packed, page id
    fs >> tmp >> tmp; // pages=1 packed=0
    fs >> tmp >> tmp; // page id=0

    // get texture filename
    std::wstring wtmp;
    fs >> wtmp; // file="Arial.png"
    startpos = wtmp.find( L"") + 1;
    m_FontImage = wtmp.substr( startpos, wtmp.size() - startpos - 1 );

    // get number of characters
    fs >> tmp >> tmp; // chars count=97
    startpos = tmp.find( L"=" ) + 1;
    int numCharacters = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );
    m_Characters.resize( numCharacters );

    for (int c = 0; c < m_Characters.size(); ++c)
    {
        // get unicode id
        fs >> tmp >> tmp; // char id=0
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].Id = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

        // get x
        fs >> tmp; // x=392
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].U = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)m_FontTexture->GetWidth();

        // get y
        fs >> tmp; // y=340
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].V = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)m_FontTexture->GetHeight();

        // get width
        fs >> tmp; // width=47
        startpos = tmp.find( L"=" ) + 1;
        tmp = tmp.substr( startpos, tmp.size() - startpos );
        m_Characters[c].Width = (float)std::stoi( tmp ) / (float)windowWidth;
        m_Characters[c].TextureWidth = (float)std::stoi( tmp ) / (float)m_FontTexture->GetWidth();

        // get height
        fs >> tmp; // height=57
        startpos = tmp.find( L"=" ) + 1;
        tmp = tmp.substr( startpos, tmp.size() - startpos );
        m_Characters[c].Height = (float)std::stoi( tmp ) / (float)windowHeight;
        m_Characters[c].TextureHeight = (float)std::stoi( tmp ) / (float)m_FontTexture->GetHeight();

        // get xoffset
        fs >> tmp; // xoffset=-6
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].XOffset = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)windowWidth;

        // get yoffset
        fs >> tmp; // yoffset=16
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].YOffset = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)windowHeight;

        // get xadvance
        fs >> tmp; // xadvance=65
        startpos = tmp.find( L"=" ) + 1;
        m_Characters[c].XAdvance = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) ) / (float)windowWidth;

        // get page
        // get channel
        fs >> tmp >> tmp; // page=0    chnl=0
    }

    // get number of kernings
    fs >> tmp >> tmp; // kernings count=96
    startpos = tmp.find( L"=" ) + 1;
    int numKernings = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );
    m_Kernings.resize( numKernings );

    for (int k = 0; k < m_Kernings.size(); ++k)
    {
        // get first character
        fs >> tmp >> tmp; // kerning first=87
        startpos = tmp.find( L"=" ) + 1;
        m_Kernings[k].FirstId = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

        // get second character
        fs >> tmp; // second=45
        startpos = tmp.find( L"=" ) + 1;
        m_Kernings[k].SecondId = std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );

        // get amount
        fs >> tmp; // amount=-1
        startpos = tmp.find( L"=" ) + 1;
        float t = (float)std::stoi( tmp.substr( startpos, tmp.size() - startpos ) );
        m_Kernings[k].Amount = (float)t / (float)windowWidth;
    }
}

float FFont::GetKerning( WChar First, WChar Second )
{
	for (size_t i = 0; i < m_Kernings.size(); ++i)
	{
		if ((WChar)m_Kernings[i].FirstId == First && (WChar)m_Kernings[i].SecondId == Second)
			return m_Kernings[i].Amount;
	}
	return 0.f;
}

const FontCharacter* FFont::GetCharacter( WChar Character )
{
	for (size_t i = 0; i < m_Characters.size(); ++i)
	{
		if (Character == (WChar)m_Characters[i].Id)
			return &m_Characters[i];
	}

	return nullptr;
}
