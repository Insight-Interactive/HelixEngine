#include "HelixEdPCH.h"

#include "Panels/DetailsPanel.h"


DetailsPanel::DetailsPanel()
{
}

DetailsPanel::~DetailsPanel()
{
}

void DetailsPanel::Initialize()
{
}

void DetailsPanel::UnInitialize()
{
}

void DetailsPanel::Tick( float DeltaTime )
{
}

void DetailsPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Details" );
	{
		/*ImGui::Text( "Hello" );

		ImGui::Separator();
		
		if (ImGui::Button( "Edit Actor" ))
		{

		}*/
	}
	ImGui::End();
}
