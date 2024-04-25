#include "HelixEdPCH.h"

#include "Panels/ConsoleOutputPanel.h"

#include "StringHelper.h"


ConsoleOutputPanel::ConsoleOutputPanel()
{
}

ConsoleOutputPanel::~ConsoleOutputPanel()
{
}

void ConsoleOutputPanel::UnInitialize()
{
}

void ConsoleOutputPanel::Tick( float DeltaTime )
{
}

void ConsoleOutputPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Console" );
	{
		if (ImGui::Button( "Clear Log" ))
		{
			Logger::FlushGlobalLogBuffer();
		}
		ImGui::Separator();

		ImGui::Text( TCharToChar( Logger::GetOutputGlobalBuffer().GetStringBuffer() ) );

		//ImGui::SetScrollY( ImGui::GetScrollMaxY() );
	}
	ImGui::End();
}
