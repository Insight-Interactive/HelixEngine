// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

/*
	Valid colors the console window text can be.
*/
enum EConsoleColor
{
	CC_Black	= 0x0000,
	CC_Blue		= 0x0001,
	CC_Green	= 0x0002,
	CC_Red		= 0x0004,
	CC_Yellow	= CC_Red	| CC_Green,
	CC_White	= CC_Red	| CC_Green | CC_Blue,
	CC_Magenta	= CC_Red	| CC_Blue,
	CC_Orange	= CC_Red	| CC_Yellow,
	CC_Cyan		= CC_Green	| CC_Blue,
};

/*
	Describes a console window for writting messages to.
*/
struct ConsoleWindowDesc
{
	// Wether the window is allowed to close.
	bool CanClose = true;
	// Output buffer dimensions.
	FVector2 BufferDims = FVector2(700, 320);
	// Window dimensions
	FVector2 WindowDims = FVector2(170, 42);
	// The default text color when adding text to the log.
	EConsoleColor DefaultForegroundColor = CC_White;
};

class ConsoleWindow
{
public:
	ConsoleWindow();
	~ConsoleWindow();

	/*
		Initialized the console window.
		@param Desc - Description describing how the window shoule be created.
	*/
	bool Create( const ConsoleWindowDesc& Desc );

	/*
		Cleanup the window and release the handles to it.
	*/
	void Destroy();

	/*
		Returns true if the console window is valid, false if not.
	*/
	bool IsValid() const;

	/*
		Returns the descriptoin the window was created with.
	*/
	const ConsoleWindowDesc& GetDesc() const;

	/*
		Returns the handle to the window.
	*/
	HWND GetWindowHandle() const;

	/*
		Returns the handle to the menu bar of the window.
	*/
	HMENU GetMenuHandle() const;

	/*
		Returns a handle to the printable surface of the window.
	*/
	HANDLE GetOutputHandle() const;

	/*
		Sets the color of the logger text on screen. All messages proceeding this call
		will be in the specified color. Call ConsoleWindow::ResetColors to revert back to defaults.
		@param NewColor - An enum value specifying the new color to change too.
	*/
	inline void SetForegroundColor( EConsoleColor NewColor );

	/*
		Resets the output colors to the windows description's specified default value.
	*/
	void ResetColors();

protected:

protected:
	ConsoleWindowDesc m_Desc;
	HWND m_WindowHandle;
	HMENU m_WindowHMenu;
	HANDLE m_OutputHandle;

};


//
// Inline function implementations
//

inline bool ConsoleWindow::IsValid() const
{
	return IsWindow(m_WindowHandle) && m_OutputHandle && m_WindowHMenu;
}

inline const ConsoleWindowDesc& ConsoleWindow::GetDesc() const 
{
	return m_Desc; 
}

inline HWND	ConsoleWindow::GetWindowHandle() const
{ 
	return m_WindowHandle; 
}

inline HMENU ConsoleWindow::GetMenuHandle()	const 
{
	return m_WindowHMenu; 
}

inline HANDLE ConsoleWindow::GetOutputHandle()	const 
{ 
	return m_OutputHandle; 
}

inline void ConsoleWindow::SetForegroundColor( EConsoleColor NewColor )
{
	SetConsoleTextAttribute( GetOutputHandle(), static_cast<WORD>(NewColor) );
}

inline void ConsoleWindow::ResetColors()
{
	SetForegroundColor( m_Desc.DefaultForegroundColor );
}