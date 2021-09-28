#include "HelixEdPCH.h"

#include "Developer/ConsoleWindow.h"


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
	m_Desc = Desc;

	// Our temp console info struct
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;

	// Get the console info and set the number of lines
	AllocConsole();
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &ConsoleInfo );
	ConsoleInfo.dwSize.Y = (SHORT)m_Desc.BufferDims.x;
	ConsoleInfo.dwSize.X = (SHORT)m_Desc.BufferDims.y;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), ConsoleInfo.dwSize );

	m_OutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );

	SMALL_RECT Rect;
	Rect.Left = 0;
	Rect.Top = 0;
	Rect.Right = (SHORT)m_Desc.WindowDims.x;
	Rect.Bottom = (SHORT)m_Desc.WindowDims.y;
	SetConsoleWindowInfo( m_OutputHandle, TRUE, &Rect );

	FILE* stream;
	freopen_s( &stream, "CONIN$", "r", stdin );
	freopen_s( &stream, "CONOUT$", "w", stdout );
	freopen_s( &stream, "CONOUT$", "w", stderr );

	if ( !m_Desc.CanClose )
	{
		// Prevent accidental console window close
		m_WindowHandle = GetConsoleWindow();
		m_WindowHMenu = GetSystemMenu( m_WindowHandle, FALSE );
		EnableMenuItem( m_WindowHMenu, SC_CLOSE, MF_GRAYED );
	}

	// Set the default console color.
	SetForegroundColor( m_Desc.DefaultForegroundColor );

	bool Valid = IsWindow( m_WindowHandle ) && m_OutputHandle && m_WindowHMenu;
	return Valid;
}

void ConsoleWindow::Destroy()
{
	if (IsValid())
	{
		CloseWindow( m_WindowHandle );
		DestroyWindow( m_WindowHandle );
		m_WindowHandle = NULL;
		DestroyMenu( m_WindowHMenu );
		CloseHandle( m_OutputHandle );
	}
}
