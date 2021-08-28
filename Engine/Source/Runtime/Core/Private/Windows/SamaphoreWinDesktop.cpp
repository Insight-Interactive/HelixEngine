#include "CorePCH.h"

#include "Semaphore.h"

Semaphore::Semaphore(const uint32 InitialCount, const uint32 MaxWaiters, const uint32 MaxCount)
{
	m_Semaphore = CreateSemaphore(NULL, InitialCount, MaxCount, NULL);
	HE_ASSERT(m_Semaphore != INVALID_HANDLE_VALUE);
}

Semaphore::~Semaphore()
{
	BOOL Result = CloseHandle(m_Semaphore);
	HE_ASSERT(Result != FALSE);
}

void Semaphore::Aquire()
{
	DWORD Result = WaitForSingleObject(m_Semaphore, INFINITE);
	HE_ASSERT(Result == WAIT_OBJECT_0);
}

bool Semaphore::TryAquire()
{
	DWORD Result = WaitForSingleObject(m_Semaphore, 0);
	return Result == WAIT_OBJECT_0;
}

void Semaphore::Release(const uint32 Count)
{
	BOOL Result = ReleaseSemaphore(m_Semaphore, Count, NULL);
	HE_ASSERT(Result == TRUE);
}

void Semaphore::Reset()
{
	do
	{
	} while (TryAquire());
}
