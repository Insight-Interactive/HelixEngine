// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"

#include "Callbacks.h"


namespace System
{
#define IE_DECLARE_HANDLE(name) struct name##__{ int Unused; }; typedef struct name##__ *name
	IE_DECLARE_HANDLE(DLLHandle);

	const uint32 kDefaultStackSize = HE_MEGABYTES(32);
	enum
	{
		kJoinable = 0x01,
		kDetached = 0x02,
	};

	struct ThreadId
	{
#if HE_WINDOWS
		DWORD Id;
		HANDLE Handle;

		FORCEINLINE bool operator == (const ThreadId& rhs) { return rhs.Id == this->Id; }
		FORCEINLINE bool operator != (const ThreadId& rhs) { return rhs.Id != this->Id; }
#endif
	};

	enum MessageDialogResult
	{
		MDR_Ok =
#if HE_WINDOWS_DESKTOP
		IDOK,
#elif HE_WINDOWS_UNIVERSAL
		0,
#endif
		MDR_No =
#if HE_WINDOWS_DESKTOP
		IDNO,
#elif HE_WINDOWS_UNIVERSAL
		0,
#endif
		MDR_Cancel = 
#if HE_WINDOWS_DESKTOP
		IDCANCEL,
#elif HE_WINDOWS_UNIVERSAL
		0,
#endif
	};
	enum MessageDialogInput
	{
		MDI_Ok = 
#if HE_WINDOWS_DESKTOP
		MB_OK,
#elif HE_WINDOWS_UNIVERSAL
		0,
#endif
		MDI_OkCancel = 
#if HE_WINDOWS_DESKTOP
		MB_OKCANCEL,
#elif HE_WINDOWS_UNIVERSAL
		0,
#endif
	};

	/*
		Initialize any platform system dependencies. Returns true if succeeded, false if not.
	*/
	bool InitializePlatform();

	/*
		Uninitialize any platform system dependencies. Returns true if succeeded, false if not.
	*/
	bool UninitializePlatform();

	/*
		Create and run a new thread.
	*/
	ThreadId CreateAndRunThread(const char* Name, const uint32 CoreIndex, JobEntryPointFn EntryPoint, void* UserData = NULL, const uint64 StackSize = kDefaultStackSize, const int32 Flags = kJoinable);

	/*
		Join an existing thread to this thread.
	*/
	void JoinThread(ThreadId Thread);

	/*
		Set the debug name for an existing thread.
	*/
	void SetThreadName(ThreadId Thread, const char* NewName);

	/*
		Process any messages from the system. MUST be called once per frame, 
		ideally at the beginning of the update loop.
	*/
	void ProcessMessages();

	/*
		Create a message box to be immediatly displayed to the user. Creating a message box is a blocking call.
		@param Message - The message to display inside the message box.
		@param Title - The message to be displayer in the titlebar of the message box window.
		@param Type - The type of message box to be displayed.
		@param pParentWindow - A pointer to the native window's handle.
	*/
	MessageDialogResult CreateMessageBox(const WChar* Message, const WChar* Title, MessageDialogInput Type, void* pParentWindow = NULL);

	/*
		Loads a DLL into the curent application's address space.
		@param Filepath - The filepath to the dll relative to the exe.
		@returns A handle to the newly loaded dll. Null if file not found or a load error occured.
	*/
	DLLHandle LoadDynamicLibrary(const wchar_t* Filepath);

	/*
		Frees a DLL form the address space.
		@param Handle - The handle to the DLL to be unloaded.
		@returns A non-zero value if the funtion succeeds.
	*/
	int32 FreeDynamicLibrary(DLLHandle Handle);

	/*
		Formats and returns the last error message the platform produced.
		@returns The formatted message.
	*/
	TChar* GetLastSystemError();

	/*
		Get the key state for a given key. Returns true if it is pressed, false if not.
		@param Key - Key to fetch the state for.
	*/
	bool GetAsyncKeyState( int32 Key );

	/*
		Returns the last system error code.
		@returns The error code of the last error the system produced.
	*/
	uint32 GetLastSystemErrorCode();

	/*
		Sets the systems last error code.
		@param NewCode - The new error code to set.
	*/
	void SetLastSystemErrorCode(uint32 NewCode);

	/*
		Gets the current working directory of the application, not including the name of the executable.
		@param [in] BufferSize - The size of `Buffer` parameter in bytes.
		@param [out] Buffer - The character buffer to pupulate with the path.
	*/
	void GetWorkingDirectory(size_t BufferSize, TChar* Buffer);

	/*
		Set the working directory of the application.
		@param Path - The new working directory path.
	*/
	void SetWorkingDirectory(TChar* Path);

	/*
		Returns the number of CPU cores the current thread posseses.
	*/
	uint32 GetProcessorCount();

	/*
		Sleep the current thread for a specified number of miliseconds
		@param Miliseconds - Time to sleep for.
	*/
	void Sleep(uint32 Miliseconds);

	/*
		Returns a pointer to this thread's command line.
	*/
	Char* GetProcessCommandLine();

	/*
		Quiries the systems high performance system timer and returns the result.
	*/
	int64 QueryPerfCounter();

	/*
		Quiries the systems CPU tick frequency and returns the result.
	*/
	int64 QueryPerfFrequency();
}
