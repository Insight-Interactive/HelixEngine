// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "App/FApp.h"
#include "CommandLine.h"
#include "RenderContext.h"
#include "RendererInitializer.h"
#include "Engine/ViewportContext.h"


class WindowClosedEvent;

class HEngine
{
	HE_DECL_NON_COPYABLE( HEngine );
public:
	HEngine();
	virtual ~HEngine();

	void EngineMain();
	void RequestShutdown();

	/*
		Returns true if the engine and all of its subsystems have been initialized. False if not.
	*/
	bool IsInitialized() const;

	/*
		Returns a reference to the main client window. This will house the scene 
		viewport in HelixEd or the main game in Shipping builds.
	*/
	ViewportContext& GetClientViewport();
	
	/*
		Returns the time between frame buffer flips.
	*/
	float GetDeltaTime() const;


protected:
	virtual void Startup();
	virtual void PostStartup();

	void Update();
	void BackgroundUpdate();

	virtual void RenderClientViewport( float DeltaTime );

	virtual void PreShutdown();
	virtual void Shutdown();
	virtual void PostShutdown();


	// Event Processing
	//
	virtual void OnEvent( Event& e );
	bool OnClientWindowClosed( WindowClosedEvent& e );

private:
	bool				m_IsInitialized;
	ViewportContext		m_MainViewPort;
	RendererInitializer	m_RenderContextInitializer;
	RenderContext		m_RenderContext;
	FApp				m_Application;

private:

	struct FrameTimeManager
	{
	public:
		FrameTimeManager() 
			: m_FrameTime(0.f)
			, m_FrameStartTick(0)
			, m_CpuTickDelta(0)
		{
		}
		~FrameTimeManager() 
		{
		}
		
		void Initialize();
		void Update( bool VSyncEnabled, bool LimitTo30Hz );

		inline float GetFrameTime() const;
		double GetCurrentTick();

	private:
		inline double TicksToSeconds( int64_t TickCount );
		inline double TicksToMillisecs( int64_t TickCount );
		inline double TimeBetweenTicks( int64_t tick1, int64_t tick2 );

	private:
		float m_FrameTime;

		int64 m_FrameStartTick;
		double m_CpuTickDelta;

	} m_FrameTimeManager;
};

extern HEngine* GEngine;


//
// Inline function implementations
//

// Engine
//

inline bool HEngine::IsInitialized() const
{
	return m_IsInitialized;
}

inline ViewportContext& HEngine::GetClientViewport()
{
	return m_MainViewPort;
}

inline float HEngine::GetDeltaTime() const
{
	return m_FrameTimeManager.GetFrameTime();
}

// Frametime manager
//

inline float HEngine::FrameTimeManager::GetFrameTime() const
{
	return m_FrameTime;
}

inline double HEngine::FrameTimeManager::TicksToSeconds( int64 TickCount )
{
	return double( TickCount ) * m_CpuTickDelta;
}

inline double HEngine::FrameTimeManager::TicksToMillisecs( int64 TickCount )
{
	return TickCount * m_CpuTickDelta * 1000.0;
}

inline double HEngine::FrameTimeManager::TimeBetweenTicks( int64 tick1, int64 tick2 )
{
	return TicksToSeconds( tick2 - tick1 );
}