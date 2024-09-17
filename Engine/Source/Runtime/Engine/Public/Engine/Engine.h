// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "App/App.h"
#include "CommandLine.h"
#include "World/World.h"
#include "Engine/Renderer/RenderContext.h"
#include "Engine/GameProject.h"
#include "Engine/ViewportContext.h"
#include "Engine/Timer.h"
#include "AssetRegistry/AssetDatabase.h"
#include "Engine/Event/EventEmitter.h"
#include "LuaScriptVM.h"
#include "SystemTime.h"
#include "Engine/Physics/Physics.h"


class WindowClosedEvent;
class CommandLine;
class HWorld;
class ThreadPool;
class EngineEvent;

class HEngine : public EventEmitter<void, EngineEvent&>
{
	friend class HEngineLaunchBootstraper;
	friend class HWorld;
public:
	/*
		Requests a global shutdown from the engine. Cleaning up and shutting down everything.
	*/
	void RequestShutdown();

	/*
		Returns true if the engine and all of its subsystems have been initialized. False if not.
	*/
	bool IsInitialized() const;

	/*
		Returns a reference to the main client window. This will house the scene 
		viewport in HelixEd or the main game in Shipping builds.
	*/
	FViewportContext& GetClientViewport();

	/*
		Returns a reference to the main game world.
	*/
	HWorld& GetGameWorld();
	
	/*
		Pauses and unpauses the game logic. Note: Takes one frame for physics simulation to pause.
	*/
	void TogglePauseGame( bool GameIsPaused );

	/*
		Returns the time between frame buffer flips.
	*/
	float GetDeltaTime() const;

	/*
		Returns the unscaled time between frame buffer flips.
	*/
	float GetFrameTime() const;

	/*
		Returns the scale of delta time.
	*/
	float GetDeltaTimeScale() const;

	/*
		Scales the delta time value used for the game
	*/
	void SetDeltaTimeScale( float Scale );

	/*
		Returns the time in seconds since app launch.
	*/
	double GetAppSeconds() const;

	/*
		Returns true if the editor is present, false if not. Editor is present if "-launchcfg LaunchEditor" is 
		passed in the command line or in standalone game builds.
	*/
	bool GetIsEditorPresent() const;

	/*
		True if the the engine is running a game simulation in the editor (or at all), false if not.
	*/
	bool IsPlayingInEditor() const;

protected:
	HEngine( FCommandLine& CmdLine );
	virtual ~HEngine();
	HE_DECL_NON_COPYABLE( HEngine );

	/*
		Main entry point for the engine. Should only ever be called once.
	*/
	void EngineMain();

	// Initializes all core components of the engine's runtime 
	// needed to load and play the game world.
	virtual void PreStartup();
	// Loads the game world and all actors and assets needed inside it.
	virtual void Startup();
	// Shows the main window and performs any last minute initialization before the presentation 
	// of the first rendered frame.
	virtual void PostStartup();

	// Ticks the engine and all subsystems associated with it.
	void Tick();
	void BackgroundUpdate( float DeltaTime );
	// Ticks the main engine timers.
	void TickTimers();

	virtual void RenderClientViewport( float DeltaTime );

	virtual void PreShutdown();
	virtual void Shutdown();
	virtual void PostShutdown();

	/*
		Set if the engine is currently simulating the game in editor. Value 
		is always set to true in standalone game builds regardless of input.
		@param IsPlaying: Wether or not the game is playing.
	*/
	void SetIsPlayingInEditor( bool IsPlaying );

	// Event Processing
	//
	virtual void OnEvent( Event& e );
	bool OnClientWindowClosed( WindowClosedEvent& e );
	bool OnWindowFocus( WindowFocusEvent& e );
	bool OnWindowLostFocus( WindowLostFocusEvent& e );


protected:
	bool					m_IsInitialized;
	bool					m_IsEditorPresent;
	bool					m_IsPlayingInEditor;
	
	// Timing
	float					m_FrameTime;
	int64					m_FrameStartTick;
	float					m_FrameTimeScale;
	double					m_AppSeconds;

	// Game world and main client viewport
	FViewportContext		m_MainViewPort;
	HWorld					m_GameWorld;

	// App/Project
	FApp					m_Application;
	FGameProject			m_GameProject;
	FAssetDatabase			m_AssetDatabase;

	// Subsystems
	FRenderContext		m_RenderContext;
	LuaScriptVM			m_ScriptSubsystem;
};

// Global engine reference. Accessible in all engine build configurtions.
extern HEngine* GEngine;
extern ThreadPool* GThreadPool;


//
// Inline function implementations
//

FORCEINLINE bool HEngine::IsInitialized() const
{
	return m_IsInitialized;
}

FORCEINLINE FViewportContext& HEngine::GetClientViewport()
{
	return m_MainViewPort;
}

FORCEINLINE HWorld& HEngine::GetGameWorld()
{
	return m_GameWorld;
}

FORCEINLINE void HEngine::TogglePauseGame( bool GameIsPaused )
{
	if (GameIsPaused)
	{
		SetDeltaTimeScale( 0.f );
		Physics::PauseSimulation();
	}
	else
	{
		SetDeltaTimeScale( 1.f );
		Physics::ResumeSimulation();
	}
}

FORCEINLINE float HEngine::GetDeltaTime() const
{
	const float MaxGameFPS = 100.f;
	return (1 / MaxGameFPS) * m_FrameTimeScale;
}

FORCEINLINE float HEngine::GetDeltaTimeScale() const
{
	return m_FrameTimeScale;
}

FORCEINLINE float HEngine::GetFrameTime() const
{
	return m_FrameTime;
}

FORCEINLINE	void HEngine::SetDeltaTimeScale( float Scale )
{
	m_FrameTimeScale = Scale;
}

FORCEINLINE double HEngine::GetAppSeconds() const
{
	return m_AppSeconds;
}

FORCEINLINE void HEngine::TickTimers()
{
	int64 CurrentTick = SystemTime::GetCurrentTick();
	m_FrameTime = (float)SystemTime::TimeBetweenTicks( m_FrameStartTick, CurrentTick );
	if (m_FrameTime > 0.25f) // Clamp the frame time if its anything less than 4 frames
		m_FrameTime = 0.25f;

	m_FrameStartTick = CurrentTick;
}

FORCEINLINE bool HEngine::GetIsEditorPresent() const
{
#if HE_STANDALONE
	return false;
#else
	return m_IsEditorPresent;
#endif
}

FORCEINLINE bool HEngine::IsPlayingInEditor() const
{
#if HE_STANDALONE
	return true;
#else
	return m_IsPlayingInEditor;
#endif
}

FORCEINLINE void HEngine::SetIsPlayingInEditor( bool IsPlaying )
{
#if HE_STANDALONE
	m_IsPlayingInEditor = true;
#else
	m_IsPlayingInEditor = IsPlaying;
#endif

	if (m_IsPlayingInEditor)
	{
		HE_LOG( Log, TEXT( "Starting Play In Editor session." ) );
	}
	else
	{
		HE_LOG( Log, TEXT( "Ending Play In Editor session." ) );
	}
}
