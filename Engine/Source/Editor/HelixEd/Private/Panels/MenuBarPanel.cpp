#include "HelixEdPCH.h"

#include "Panels/MenuBarPanel.h"


MenuBarPanel::MenuBarPanel()
{

}

MenuBarPanel::~MenuBarPanel()
{

}

void MenuBarPanel::Initialize()
{

}

void MenuBarPanel::UnInitialize()
{

}

void MenuBarPanel::Tick( float DeltaTime )
{

}

void MenuBarPanel::Render( ICommandContext& CmdCtx )
{
	if (ImGui::BeginMainMenuBar())
	{
		for (auto MenuIter = m_MenuItems.begin(); MenuIter != m_MenuItems.end(); MenuIter++)
		{
			if (ImGui::BeginMenu( MenuIter->first.c_str() ))
			{
				for (auto MenuItemIter = MenuIter->second.begin(); MenuItemIter != MenuIter->second.end(); MenuItemIter++) 
				{
					if (ImGui::MenuItem( MenuItemIter->first.c_str() ))
					{
						MenuItemIter->second(  );
					}
				}

				ImGui::EndMenu();
			}
			ImGui::Separator();
		}

		ImGui::EndMainMenuBar();
	}
}
