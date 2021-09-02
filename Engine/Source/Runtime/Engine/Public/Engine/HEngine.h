// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "App/FApp.h"
#include "CommandLine.h"
#include "RenderContext.h"
#include "RendererInitializer.h"
#include "Engine/ViewportContext.h"
#include "AssetRegistry/AssetDatabase.h"


class WindowClosedEvent;
class CommandLine;

class HEngine
{
	HE_DECL_NON_COPYABLE( HEngine );
public:
	HEngine(CommandLine& CmdLine);
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

	/*
		Returns true if the editor is present, false if not.
	*/
	bool GetIsEditorPresent();

	/*
		True if the the engine is running a simulation in the editor, false if not.
	*/
	//bool IsPlayingInEditor();

	//void SetIsPlayingInEditor( bool IsPlaying );


protected:
	virtual void PreStartup();
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
	bool				m_IsEditorPresent;
	bool				m_IsPlayingInEditor;
	ViewportContext		m_MainViewPort;
	RendererInitializer	m_RenderContextInitializer;
	RenderContext		m_RenderContext;
	FApp				m_Application;
	AssetDatabase		m_AssetDatabase;

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

inline bool HEngine::GetIsEditorPresent()
{
	return m_IsEditorPresent;
}

//inline bool HEngine::IsPlayingInEditor()
//{
//	return m_IsPlayingInEditor;
//}
//
//inline void HEngine::SetIsPlayingInEditor( bool IsPlaying )
//{
//	m_IsPlayingInEditor = IsPlaying;
//
//	if (m_IsPlayingInEditor)
//	{
//		HE_LOG( Log, TEXT( "Starting Play In Editor session." ) );
//	}
//	else
//	{
//		HE_LOG( Log, TEXT( "Ending Play In Editor session." ) );
//	}
//}

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