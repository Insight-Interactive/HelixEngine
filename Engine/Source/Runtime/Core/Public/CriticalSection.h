#pragma once

#include "CoreFwd.h"

class CORE_API CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	bool TryEnter();
	void Exit();
	
private:
#if HE_WINDOWS
	CRITICAL_SECTION m_Section;
#endif
};


/*
	Enters and exits a critical section based on a scoped lifetime (RAII)
*/
struct ScopedCriticalSection
{
	ScopedCriticalSection(CriticalSection& Section)
		: m_Section(Section)
	{
		m_Section.Enter();
	}

	~ScopedCriticalSection()
	{
		m_Section.Exit();
	}
private:
	CriticalSection& m_Section;

};
