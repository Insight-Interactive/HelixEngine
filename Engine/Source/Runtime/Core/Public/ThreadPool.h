#pragma once

#include "System.h"

#include <thread>

#include "CriticalSection.h"
#include "Semaphore.h"
#include "Callbacks.h"
#include "Flag.h"

class ThreadPool
{
public:
	ThreadPool(uint32 NumWorkers, const uint32* CoreAffinities, uint32 MaxQueueSize = 16, const uint64 MaxStackSize = System::kDefaultStackSize);
	~ThreadPool();

	void SpawnWorkers();

	const FFlag& Kick(JobEntryPointFn pEntryPoint, void* pUserData);

protected:
	static void WorkerThreadEntryPoint(void* pUserData);
	void WorkerThread();

	struct Worker
	{
		System::ThreadId Thread;
	};
	struct Job
	{
		Job()
			: Flags(0u)
			, pUserData(NULL)
			, JobMain(NULL)
		{
		}
		uint64 Flags;
		void* pUserData;
		JobEntryPointFn	JobMain;
		FFlag OnCompletedFlag;
	};
	struct JobQueue
	{
		const uint32 kInvalidJobQueueSize = (uint32)-1;
		JobQueue()
			: m_Queue(NULL)
			, m_AddIndex(0)
			, m_TakeIndex(0)
			, m_MaxQueueSize(kInvalidJobQueueSize)
		{
		}
		~JobQueue()
		{
			if (IsValid())
				UnInitialize();
		}

		inline void Initialize(uint32 MaxQueueSize);
		inline void UnInitialize();

		inline bool IsValid();
		inline Job& operator[](uint32 Index);

		CriticalSection Guard;
		Semaphore		Counter;

		Job* m_Queue;
		uint32			m_AddIndex;
		uint32			m_TakeIndex;
		uint32			m_MaxQueueSize;
	};
	enum
	{
		kMaxParallelThreads = 12,
		kInvalidAffinity = 0xFFFFFFFF,
	};
	enum JobUserDataValue
	{
		JobFuntionPointer,
		JobRef,
		JobQuitRequest,
	};
	const FFlag& KickInternal(const Job NewJob);


	uint32	m_NumWorkerThreads;
	Worker* m_Workers;
	uint64	m_StackSize;
	uint32	m_CoreAffinities[kMaxParallelThreads];

	JobQueue		m_JobQueue;
};




//
// Inline Function Implementations
//


inline void ThreadPool::JobQueue::Initialize(uint32 MaxQueueSize)
{
	UnInitialize();

	m_AddIndex = 0;
	m_TakeIndex = 0;
	m_MaxQueueSize = MaxQueueSize;
	m_Queue = new Job[MaxQueueSize];
}

inline void ThreadPool::JobQueue::UnInitialize()
{
	if (IsValid())
	{
		HE_SAFE_DELETE_PTR_ARRAY(m_Queue);
		m_MaxQueueSize = kInvalidJobQueueSize;
	}
}

inline bool ThreadPool::JobQueue::IsValid()
{
	return (m_Queue != NULL) && (m_MaxQueueSize != kInvalidJobQueueSize);
}

inline ThreadPool::Job& ThreadPool::JobQueue::operator[](uint32 Index)
{
	return m_Queue[Index];
}
