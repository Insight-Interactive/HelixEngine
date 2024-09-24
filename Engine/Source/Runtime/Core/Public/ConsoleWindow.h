#pragma once

#include "Windows.h"


struct ConsoleWindowDesc
{

	bool CanClose = true;

	uint32 BufferDimsX;
	uint32 BufferDimsY;

	uint32 WindowDimsX;
	uint32 WindowDimsY;
};

class ConsoleWindow
{
public:
	ConsoleWindow();
	~ConsoleWindow();

	bool Create( const ConsoleWindowDesc& Desc );
	void Destroy();
	bool IsValid() { return m_WindowHandle != NULL && m_OutputHandle != NULL; }

protected:
	HWND m_WindowHandle;
	HMENU m_WindowHMenu;
	HANDLE m_OutputHandle;
};