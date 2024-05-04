// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"

#include "Callbacks.h"
#include "System.h"


class FFixedLoop
{
public:
	FFixedLoop();
	~FFixedLoop();

	template <class ClassTarget, typename MethodTarget>
	void Do( const float& StepRate, ClassTarget* pClass, void(MethodTarget::* pMemberFn)(void) );

	void Do( const float& StepRate, OutVoidInVoidFn Fn );


private:
	float m_AccumulatedTime;
	int64 m_LastTime;

};

//
// Inline function implementations
//


FORCEINLINE FFixedLoop::FFixedLoop()
	: m_AccumulatedTime( 0.f )
	, m_LastTime( 0 )
{
	m_LastTime = System::QueryPerfCounter();
}

FORCEINLINE FFixedLoop::~FFixedLoop()
{
}

template <class ClassTarget, typename MethodTarget>
FORCEINLINE void FFixedLoop::Do( const float& StepRate, ClassTarget* pClass, void(MethodTarget::* pMemberFn)(void) )
{
	const int64 CurrentTime = System::QueryPerfCounter();
	const float DeltaTime = (CurrentTime - m_LastTime) / (float)System::QueryPerfFrequency();

	m_AccumulatedTime += DeltaTime;
	while (m_AccumulatedTime > StepRate)
	{
		HE_INVOKE_MEMBER_FN( pClass, pMemberFn );
		m_AccumulatedTime -= StepRate;
	}
	m_LastTime = CurrentTime;
}

FORCEINLINE void FFixedLoop::Do( const float& StepRate, OutVoidInVoidFn Fn )
{
	const int64 CurrentTime = System::QueryPerfCounter();
	const float DeltaTime = (CurrentTime - m_LastTime) / (float)System::QueryPerfFrequency();

	m_AccumulatedTime += DeltaTime;
	while (m_AccumulatedTime > StepRate)
	{
		Fn();
		m_AccumulatedTime -= StepRate;
	}
	m_LastTime = CurrentTime;
}
