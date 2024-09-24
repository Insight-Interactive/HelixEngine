#include "CorePCH.h"

#include "ConsoleWindow.h"


ConsoleWindow::ConsoleWindow()
	: m_WindowHandle( NULL )
	, m_OutputHandle( NULL )
	, m_WindowHMenu( NULL )
{
}

ConsoleWindow::~ConsoleWindow()
{
	Destroy();
}

bool ConsoleWindow::Create( const ConsoleWindowDesc& Desc )
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	// Get the console info and set the number of lines
	AllocConsole();
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
	coninfo.dwSize.Y = (SHORT)Desc.BufferDimsX;
	coninfo.dwSize.X = (SHORT)Desc.BufferDimsY;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );

	m_OutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );

	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = (SHORT)Desc.WindowDimsX;
	rect.Bottom = (SHORT)Desc.WindowDimsY;
	SetConsoleWindowInfo( m_OutputHandle, TRUE, &rect );

	FILE* stream;
	freopen_s( &stream, "CONIN$", "r", stdin );
	freopen_s( &stream, "CONOUT$", "w", stdout );
	freopen_s( &stream, "CONOUT$", "w", stderr );

	if (!Desc.CanClose)
	{
		// Prevent accidental console window close
		m_WindowHandle = GetConsoleWindow();
		m_WindowHMenu = GetSystemMenu( m_WindowHandle, FALSE );
		EnableMenuItem( m_WindowHMenu, SC_CLOSE, MF_GRAYED );
	}

	return IsWindow( m_WindowHandle ) && m_OutputHandle && m_WindowHMenu;
}

void ConsoleWindow::Destroy()
{
	DestroyWindow( m_WindowHandle );
	DestroyMenu( m_WindowHMenu );
	CloseHandle( m_OutputHandle );
}
