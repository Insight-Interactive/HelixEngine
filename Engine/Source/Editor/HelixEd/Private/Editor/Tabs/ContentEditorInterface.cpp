#include "HelixEdPCH.h"

#include "Editor/Tabs/ContentEditorInterface.h"


ContentEditorInterface::ContentEditorInterface( const TChar* Title )
	: m_IsOpen( false )
	, m_UIContext( *this )
{
	FWindow::Description ClientDesc = {};
	ClientDesc.bHasTitleBar		= true;
	ClientDesc.bShowImmediate	= true;
	ClientDesc.Resolution		= GCommonResolutions[k900p]; ClientDesc.Resolution.Width += 230; ClientDesc.Resolution.Height += 200;
	ClientDesc.Title			= Title;
	ClientDesc.bAllowDropFiles	= true;
	Super::Initialize( ClientDesc );
}

ContentEditorInterface::~ContentEditorInterface()
{

}

void ContentEditorInterface::Activate()
{
	SetupPanels();
	OnActivated();
}

void ContentEditorInterface::Deactivate()
{
	TeardownPanels();
	OnDeactivated();
}

void ContentEditorInterface::SetupPanels()
{
}

void ContentEditorInterface::TeardownPanels()
{
}

void ContentEditorInterface::CreateUIContext()
{
	m_UIContext.Setup();
}

void ContentEditorInterface::Tick( float DeltaTime )
{
	Super::Update( DeltaTime );
}

void ContentEditorInterface::Render( FCommandContext& CmdContext )
{
	Super::Render();
}

void ContentEditorInterface::OnEvent( Event& e )
{
}
