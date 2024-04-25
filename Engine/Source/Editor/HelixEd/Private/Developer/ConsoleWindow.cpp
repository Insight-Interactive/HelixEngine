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

	// Get the console info and set the number of lines
	if (!AllocConsole())
	{
		HE_LOG( Error, TEXT( "Failed to allocate console window! Console will not be created. Error code: %i" ), ::GetLastError() );
		return false;
	}

	// Temp console info struct.
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
	m_OutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( m_OutputHandle, &ConsoleInfo );
	ConsoleInfo.dwSize.Y = (SHORT)m_Desc.BufferDims.x;
	ConsoleInfo.dwSize.X = (SHORT)m_Desc.BufferDims.y;
	SetConsoleScreenBufferSize( m_OutputHandle, ConsoleInfo.dwSize );

	
	SMALL_RECT FRect;
	FRect.Left = 0;
	FRect.Top = 0;
	FRect.Right = (SHORT)m_Desc.WindowDims.x;
	FRect.Bottom = (SHORT)m_Desc.WindowDims.y;
	SetConsoleWindowInfo( m_OutputHandle, TRUE, &FRect );

	FILE* stream;
	freopen_s( &stream, "CONIN$", "r", stdin );
	freopen_s( &stream, "CONOUT$", "w", stdout );
	freopen_s( &stream, "CONOUT$", "w", stderr );

	m_WindowHandle = GetConsoleWindow();
	m_WindowHMenu = GetSystemMenu( m_WindowHandle, FALSE );
	if ( !m_Desc.CanClose )
	{
		// Prevent console window from being closed.
		EnableMenuItem( m_WindowHMenu, SC_CLOSE, MF_GRAYED );
	}

	// Set the default console color.
	SetForegroundColor( m_Desc.DefaultForegroundColor );
	
	return IsValid();
}

void ConsoleWindow::Destroy()
{
	if (IsValid())
	{
		FreeConsole();
		CloseWindow( m_WindowHandle );
		DestroyWindow( m_WindowHandle );
		m_WindowHandle = NULL;
		DestroyMenu( m_WindowHMenu );
	}
}
