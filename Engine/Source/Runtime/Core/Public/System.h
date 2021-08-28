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
#if defined HE_WINDOWS
	{
		DWORD Id;
		HANDLE Handle;

		bool operator == (const ThreadId& rhs) { return rhs.Id == this->Id; }
		bool operator != (const ThreadId& rhs) { return rhs.Id != this->Id; }
	};
#else
	{
	};
#endif

	ThreadId CreateAndRunThread(const char* Name, const uint32 CoreIndex, JobEntryPointFn EntryPoint, void* UserData = NULL, const uint64 StackSize = kDefaultStackSize, const int32 Flags = kJoinable);

	void JoinThread(ThreadId Thread);

	void SetThreadName(ThreadId Thread, const char* NewName);

	/*
		Process any messages from the system. MUST be called once per frame, 
		ideally at the beginning of the update loop.
	*/
	void ProcessMessages();

	/*
		Create a message box to be immediatly displayed to th user.
		@param Message - The message to display inside the message box.
		@param Title - The message to be displayer in the titlebar of the message box window.
		@param pParentWindow - A pointer to the native window's handle.
	*/
	void CreateMessageBox(const wchar_t* Message, const wchar_t* Title, void* pParentWindow = NULL);

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

	Char* GetProcessCommandLine();

	int64 QueryPerformanceCounter();

	int64 QueryPerformanceFrequency();
}
