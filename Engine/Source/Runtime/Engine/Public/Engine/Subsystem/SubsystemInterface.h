#pragma once


class FSubsystemInterface
{
	friend class HEngine;
	friend class FSubsystemAsyncRunHelper;
public:

protected:
	FSubsystemInterface( Char* DebugName = "Unnamed Helix Subsystem" );
	virtual ~FSubsystemInterface();

	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;

	const Char* GetDebugName() const;
	void RunAsync();
	void TerminateAsyncProcess();

private:
	virtual void RunAsync_Implementation() {}

protected:
	bool m_IsRunning;
#if HE_DEBUG
	Char m_DebugName[64];
#endif // HE_DEBUG

};


//
// Inline function implementations
//

FORCEINLINE void FSubsystemInterface::TerminateAsyncProcess()
{
	m_IsRunning = false;
}

FORCEINLINE const Char* FSubsystemInterface::GetDebugName() const
{
	return m_DebugName;
}
