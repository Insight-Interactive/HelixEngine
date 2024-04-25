// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/FileExplorerWindow.h"


FileExplorerWindow::FileExplorerWindow()
	: m_pFileDialog( NULL )
{
	HRESULT hr = CoCreateInstance( CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS( &m_pFileDialog ) );
	HE_ASSERT( SUCCEEDED( hr ) == true );
}

FileExplorerWindow::~FileExplorerWindow()
{
	HE_COM_SAFE_RELEASE( m_pFileDialog );
}

bool FileExplorerWindow::Create( const Description& Desc )
{
	DWORD dwFlags;
	if (FAILED( m_pFileDialog->GetOptions( &dwFlags ) )) return false;
	dwFlags |= (Desc.FilterFoldersOnly) ? FOS_PICKFOLDERS : FOS_FORCEFILESYSTEM;
	if (FAILED( m_pFileDialog->SetOptions( dwFlags ) )) return false;
	if (FAILED( m_pFileDialog->SetTitle( Desc.Title ) )) return false;

	return true;
}

void FileExplorerWindow::Show( WChar** Result )
{
	HE_ASSERT( IsValid() );
	m_pFileDialog->Show( NULL );

	IShellItem* pResult = NULL;
	HRESULT hr = m_pFileDialog->GetResult( &pResult );
	if (SUCCEEDED( hr ))
	{
		pResult->GetDisplayName( SIGDN_FILESYSPATH, Result );
	}
	else
	{
		HE_LOG( Error, TEXT( "Failed to get result from file dialog! HRESULT: %i" ), hr );
		return;
	}
}

void FileExplorerWindow::Close()
{
	HE_ASSERT( IsValid() );
	m_pFileDialog->Close( HRESULT_FROM_WIN32( ERROR_CANCELLED ) );
}

bool FileExplorerWindow::IsValid()
{
	return m_pFileDialog != NULL;
}
