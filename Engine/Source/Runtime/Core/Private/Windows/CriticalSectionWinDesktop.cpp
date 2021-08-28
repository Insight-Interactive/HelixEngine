#include "CorePCH.h"

#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	BOOL Result = InitializeCriticalSectionAndSpinCount(&m_Section, 0x80000400);
	SUCCEEDED(Result);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_Section);
}

void CriticalSection::Enter()
{
	EnterCriticalSection(&m_Section);
}

bool CriticalSection::TryEnter()
{
	return TryEnterCriticalSection(&m_Section) == TRUE;
}

void CriticalSection::Exit()
{
	LeaveCriticalSection(&m_Section);
}
