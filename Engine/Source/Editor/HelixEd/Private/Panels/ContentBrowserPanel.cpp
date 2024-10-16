#include "HelixEdPCH.h"

#include "Panels/ContentBrowserPanel.h"

#include "StringHelper.h"
#include "System.h"
#include "Engine/GameProject.h"
#include "Editor/Event/EditorEvent.h"


ContentBrowserPanel::ContentBrowserPanel()
{

}

ContentBrowserPanel::~ContentBrowserPanel()
{

}

void ContentBrowserPanel::Initialize( FViewportContext* pOwningTab )
{
	Super::Initialize( pOwningTab );

	const String & GameProjectRoot = FGameProject::GetInstance()->GetContentFolder();
	m_ProjectContentRoot = CharToTChar( GameProjectRoot );
	m_ProjectContentRoot.append(TEXT("\\*"));
}

void ContentBrowserPanel::UnInitialize()
{

}

void ContentBrowserPanel::Tick( float DeltaTime )
{

}

void ContentBrowserPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Content Browser" );
	{
		ImGuiTreeNodeFlags FolderNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (ImGui::TreeNodeEx( "Content", ImGuiTreeNodeFlags_DefaultOpen ))
		{
			TraverseFolder( m_ProjectContentRoot.c_str() );

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ContentBrowserPanel::TraverseFolder( const TChar* Folder )
{
	HName Root = Folder;

	if (Root.back() == '*') Root.erase( Root.end() - 1 );
	if (Root.back() != '\\') Root.push_back( '\\' );

	WIN32_FIND_DATA File;
	HANDLE hSearch = FindFirstFile( Folder, &File );
	
	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			String FileName = StringHelper::UTF16ToUTF8( File.cFileName );
			if(FileName == "." || FileName == ".." )
				continue;
			
			if (File.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				const ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				if (ImGui::TreeNodeEx( FileName.c_str(), NodeFlags ))
				{
					HName NextPath = Root;
					NextPath += File.cFileName;
					NextPath += TEXT( "\\*" );
					TraverseFolder( NextPath.c_str() );

					ImGui::TreePop();
				}
			}
			else
			{
				const ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Leaf;

				if (ImGui::TreeNodeEx( FileName.c_str(), NodeFlags ))
				{
					if (ImGui::IsItemHovered( ) && ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ))
					{
						ContentItemDoubleClicked e( File.cFileName );
						EmitEvent( e );
					}

					ImGui::TreePop();
				}

			}

		} while (FindNextFile( hSearch, &File ));

		FindClose( hSearch );
	}
}
