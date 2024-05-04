// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"

#define HE_DEFAULT_SEMAPHORE_MAX_COUNT 100000

class CORE_API Semaphore
{
public:
	Semaphore(const uint32 InitialCount = 0, const uint32 MaxWaiters = 16, const uint32 MaxCount = HE_DEFAULT_SEMAPHORE_MAX_COUNT);
	~Semaphore();

	void Aquire();
	bool TryAquire();
	void Release(const uint32 Count = 1);

	void Reset();

protected:
#if HE_WINDOWS
	HANDLE m_Semaphore;
#endif
};