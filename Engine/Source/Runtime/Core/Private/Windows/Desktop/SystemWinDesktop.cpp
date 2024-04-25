// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "CorePCH.h"

#include "System.h"


namespace System
{
	struct Win32ThreadData
	{
		JobEntryPointFn EntryPoint;
		void* UserData;
		const char* Name;
		DWORD Id;
	};

	unsigned WINAPI Win32ThreadEntry( void* pArgs )
	{
		Win32ThreadData Data = *(Win32ThreadData*)pArgs;
		delete pArgs;

		Data.EntryPoint( Data.UserData );

		return EXIT_SUCCESS;
	}

	bool InitializePlatform()
	{
		// Init the COM library.
		HRESULT hr = CoInitialize( NULL );
		return SUCCEEDED( hr );
	}

	bool UninitializePlatform()
	{
		// UnInit the COM library.
		CoUninitialize();
		
		return true;
	}

	ThreadId CreateAndRunThread( const char* Name, const uint32 CoreIndex, JobEntryPointFn EntryPoint, void* UserData/* = NULL*/, const uint64 StackSize/* = kDefaultStackSize*/, const int32 Flags/* = kJoinable*/ )
	{
		Win32ThreadData* ThreadData = new Win32ThreadData();
		ThreadData->Name = Name;
		ThreadData->Id = CoreIndex;
		ThreadData->UserData = UserData;
		ThreadData->EntryPoint = EntryPoint;

		uint32 ThreadFlags = 0;
		unsigned int NewThreadId = 0;
		HANDLE NewThreadHandle = (HANDLE)_beginthreadex( NULL, (unsigned int)StackSize, &Win32ThreadEntry, ThreadData, ThreadFlags, &NewThreadId );

		ThreadId Thread;
		Thread.Id = NewThreadId;
		Thread.Handle = NewThreadHandle;
#if HE_DEBUG_THREADS
		SetThreadName( Thread, Name );
#endif

		return Thread;
	}

	void JoinThread( ThreadId Thread )
	{
		/*DWORD ExitCode = 0;
		do
		{
			Sleep( 1 );
			BOOL Result = GetExitCodeThread(Thread.Handle, &ExitCode);
			HE_ASSERT(Result != FALSE);

		} while (ExitCode == STILL_ACTIVE);*/

		HE_ASSERT( CloseHandle( Thread.Handle ) != FALSE );
	}

	void SetThreadName( ThreadId Thread, const char* NewName )
	{
		struct ThreadNameInfo
		{
			DWORD	dwType;		// Must be 0x1000.
			LPCSTR	szName;		// Pointer to name (in user address space).
			DWORD	dwThreadID; // Thread ID (-1=caller thread).
			DWORD	dwFlags;	// Reserved for future use, must be zero.
		};

		ThreadNameInfo Info;
		Info.dwType = 0x1000;
		Info.szName = NewName;
		Info.dwThreadID = Thread.Id;
		Info.dwFlags = 0;

		__try
		{
			RaiseException( 0x406D1388, 0, sizeof( Info ) / sizeof( DWORD ), (CONST ULONG_PTR*) & Info );
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
	void ProcessMessages()
	{
		MSG Message = { 0 };
		while (::PeekMessage( &Message, NULL, 0, 0, PM_REMOVE ))
		{
			::TranslateMessage( &Message );
			::DispatchMessage( &Message );
		}
	}

	DLLHandle LoadDynamicLibrary( const wchar_t* Filepath )
	{
		return (DLLHandle) ::LoadLibrary( Filepath );
	}

	int32 FreeDynamicLibrary( DLLHandle Handle )
	{
		HE_ASSERT( Handle != NULL ); // Trying to free a handle to a null library.

		return (int32)::FreeLibrary( (HMODULE)Handle );
	}

	MessageDialogResult CreateMessageBox( const WChar* Message, const WChar* Title, MessageDialogInput Type, MessageDialogIcon Icon, void* pParentWindow/* = NULL*/ )
	{
		UINT Flags = Icon | MB_DEFBUTTON1 | MB_SYSTEMMODAL;
		return (MessageDialogResult)::MessageBox( RCast<HWND>( pParentWindow ), Message, Title, (UINT)Type | Flags );
	}

	TChar* GetLastSystemError()
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD ErrCode = ::GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			ErrCode,
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			(LPTSTR)&lpMsgBuf,
			0, NULL );

		lpDisplayBuf = (LPVOID)::LocalAlloc( LMEM_ZEROINIT, (lstrlen( (LPCTSTR)lpMsgBuf ) + lstrlen( (LPCTSTR)L"" ) + 40) * sizeof( TCHAR ) );
		StringCchPrintf(
			(LPTSTR)lpDisplayBuf,
			::LocalSize( lpDisplayBuf ) / sizeof( TCHAR ),
			TEXT( "%s failed with error %d: %s" ),
			"", ErrCode, lpMsgBuf
		);
		return (TChar*)lpDisplayBuf;
	}

	bool GetAsyncKeyState( int32 Key )
	{
		return ((1 << 15) & ::GetAsyncKeyState( Key ));
	}

	uint32 GetLastSystemErrorCode()
	{
		return ::GetLastError();
	}

	void SetLastSystemErrorCode( uint32 NewCode )
	{
		::SetLastError( NewCode );
	}

	void GetWorkingDirectory( size_t BufferSize, TChar* Buffer )
	{
		// Fetch the current directory (includes the exe name).
		DWORD Result = ::GetModuleFileName( NULL, Buffer, (DWORD)BufferSize );
		if (Result == 0)
		{
			BufferSize = -1;
			Buffer = NULL;
			return;
		}
		size_t BufferLength = TCharStrlen( Buffer );
		// Start at the end and remove the exe name character by character.
		for (size_t i = BufferLength - 1; i != 0; --i)
		{
			if (Buffer[i] != TChar( '\\' ) && Buffer[i] != TChar( '/' ))
			{
				// Erase the character
				Buffer[i] = TChar( '\0' );
			}
			else
			{
				// Found the first set of slashes; the exe has been erased.
				break;
			}
		}
	}

	void SetWorkingDirectory( TChar* Path )
	{
		bool Result = ::SetCurrentDirectory( Path );
		if (!Result)
		{
			Path = NULL;
		}
	}

	uint32 GetProcessorCount()
	{
		SYSTEM_INFO SystemInfo = {};
		::GetSystemInfo( &SystemInfo );

		return SystemInfo.dwNumberOfProcessors;
	}

	void Sleep( uint32 Miliseconds )
	{
		::Sleep( Miliseconds );
	}

	Char* GetProcessCommandLine()
	{
		return ::GetCommandLineA();
	}

	int64 QueryPerfCounter()
	{
		LARGE_INTEGER Tick = { 0 };
		HE_ASSERT( ::QueryPerformanceCounter( &Tick ) == TRUE );
		return (int64)Tick.QuadPart;
	}

	int64 QueryPerfFrequency()
	{
		LARGE_INTEGER Frequency = { 0 };
		HE_ASSERT( ::QueryPerformanceFrequency( &Frequency ) == TRUE );

		return (int64)Frequency.QuadPart;
	}
}
