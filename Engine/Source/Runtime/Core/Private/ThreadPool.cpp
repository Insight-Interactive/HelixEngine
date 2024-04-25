#include "CorePCH.h"

#include "ThreadPool.h"



ThreadPool::ThreadPool(uint32 NumWorkers, const uint32* CoreAffinities, uint32 MaxQueueSize, const uint64 MaxStackSize)
	: m_NumWorkerThreads(NumWorkers)
	, m_Workers(NULL)
	, m_StackSize(MaxStackSize)
{
	//ZeroMemory(m_CoreAffinities, sizeof(uint32) * kMaxParallelThreads);

	if (CoreAffinities != NULL)
		CopyMemory(m_CoreAffinities, CoreAffinities, sizeof(uint32) * NumWorkers);
	else
		m_CoreAffinities[0] = kInvalidAffinity;

	m_JobQueue.Initialize(MaxQueueSize);
}

ThreadPool::~ThreadPool()
{
	if (m_Workers != NULL)
	{
		Job QuitJob;
		QuitJob.Flags		= JobQuitRequest;
		QuitJob.JobMain		= NULL;
		QuitJob.pUserData	= NULL;
		
		for (uint32 i = 0; i < m_NumWorkerThreads; i++)
		{
			System::JoinThread(m_Workers[i].Thread);
		}
		
		HE_SAFE_DELETE_PTR_ARRAY(m_Workers);
	}
	m_JobQueue.UnInitialize();
}

void ThreadPool::SpawnWorkers()
{
	m_Workers = new Worker[m_NumWorkerThreads];
	for (uint32 i = 0; i < m_NumWorkerThreads; ++i)
	{
		char ThreadName[32];
#if HE_DEBUG_THREADS
		sprintf_s(ThreadName, "Helix Worker Thread %u", i);
#endif
		m_Workers[i].Thread = System::CreateAndRunThread(
			ThreadName,
			m_CoreAffinities[0] == kInvalidAffinity ? 0 : m_CoreAffinities[i],
			WorkerThreadEntryPoint,
			this,
			m_StackSize);
	}
}

const FFlag& ThreadPool::Kick(JobEntryPointFn pEntryPoint, void* pUserData)
{
	Job NewJob;
	NewJob.Flags = JobFuntionPointer;
	NewJob.JobMain = pEntryPoint;
	NewJob.pUserData = pUserData;

	return KickInternal(NewJob);
}

const FFlag& ThreadPool::KickInternal(const Job NewJob)
{
	m_JobQueue.Guard.Enter();
	{
		HE_ASSERT(m_JobQueue[m_JobQueue.m_AddIndex].Flags == 0);

		m_JobQueue[m_JobQueue.m_AddIndex] = NewJob;
		if (++m_JobQueue.m_AddIndex == m_JobQueue.m_MaxQueueSize)
		{
			m_JobQueue.m_AddIndex = 0;
		}
	}
	m_JobQueue.Guard.Exit();
	m_JobQueue.Counter.Release();

	return m_JobQueue[m_JobQueue.m_AddIndex].OnCompletedFlag;
}

void ThreadPool::WorkerThreadEntryPoint(void* pUserData)
{
	ThreadPool* pThreadPool = RCast<ThreadPool*>(pUserData);
	pThreadPool->WorkerThread();
}

void ThreadPool::WorkerThread()
{
	while (true)
	{
		// Guard resources.
		m_JobQueue.Counter.Aquire();
		m_JobQueue.Guard.Enter();

		uint64 Flags			= 0;
		JobEntryPointFn FnPtr	= NULL;
		void* pUserData			= NULL;

		// Grab another job from the queue.
		Job& KickedJob	= m_JobQueue[m_JobQueue.m_TakeIndex];
		Flags			= KickedJob.Flags;
		FnPtr			= KickedJob.JobMain;
		pUserData		= KickedJob.pUserData;

		m_JobQueue[m_JobQueue.m_TakeIndex].Flags = 0;
		if (++m_JobQueue.m_TakeIndex == m_JobQueue.m_MaxQueueSize)
		{
			m_JobQueue.m_TakeIndex = 0;
		}
		m_JobQueue.Guard.Exit();

		if (Flags & JobQuitRequest)
		{
			break;
		}
		else if (Flags & JobRef)
		{

		}
		else
		{
			//HE_ASSERT(Flags & JobFuntionPointer); // Invalid flag given when kicking job.
			FnPtr(pUserData);
			KickedJob.OnCompletedFlag.Set();
		}
	}
}
