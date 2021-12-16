// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "App/App.h"
#include "CommandLine.h"
#include "World/World.h"
#include "RenderContext.h"
#include "Engine/GameProject.h"
#include "Engine/ViewportContext.h"
#include "Engine/FrameTimeManager.h"
#include "AssetRegistry/AssetDatabase.h"
#include "Engine/Subsystem/PhysicsSubsystem.h"
#include "Engine/Subsystem/RenderingSubsystem.h"


class WindowClosedEvent;
class CommandLine;
class HWorld;
class ThreadPool;

class HEngine
{
	friend class HEngineLaunchBootstraper;
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
		Returns the time between frame buffer flips in miliseconds.
	*/
	double GetDeltaTime() const;

	/*
		Returns the time in seconds since app launch.
	*/
	double GetAppSeconds() const;

	/*
		Returns true if the editor is present, false if not. Editor is present if "-launchcfg LaunchEditor" is 
		passed in the command line or in standalone game builds.
	*/
	bool GetIsEditorPresent();

	/*
		True if the the engine is running a game simulation in the editor (or at all), false if not.
	*/
	bool IsPlayingInEditor();


protected:
	HEngine( CommandLine& CmdLine );
	virtual ~HEngine();
	HE_DECL_NON_COPYABLE( HEngine );

	/*
		Main entry point for the engine. Should only ever be called once.
	*/
	void EngineMain();

	virtual void PreStartup();
	virtual void Startup();
	virtual void PostStartup();

	void Update();
	void BackgroundUpdate( float DeltaTime );
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

protected:
	bool					m_IsInitialized;
	bool					m_IsEditorPresent;
	bool					m_IsPlayingInEditor;
	FFrameTimer				m_FrameTimer;
	double					m_AppSeconds;
	FViewportContext		m_MainViewPort;
	FRenderContext			m_RenderContext;
	FApp					m_Application;
	FGameProject			m_GameProject;
	FAssetDatabase			m_AssetDatabase;
	HWorld					m_GameWorld;

	// Subsystems
	FPhysicsSubsystem		m_PhysicsSubsystem;
	FRenderingSubsystem		m_ReneringSubsystem;
};

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

FORCEINLINE double HEngine::GetDeltaTime() const
{
	return m_FrameTimer.GetTimeMiliSeconds();
}

FORCEINLINE double HEngine::GetAppSeconds() const
{
	return m_AppSeconds;
}

FORCEINLINE void HEngine::TickTimers()
{
	m_FrameTimer.Tick();
	m_AppSeconds += m_FrameTimer.GetTimeMiliSeconds();
}

FORCEINLINE bool HEngine::GetIsEditorPresent()
{
#if HE_STANDALONE
	return false;
#else
	return m_IsEditorPresent;
#endif
}

FORCEINLINE bool HEngine::IsPlayingInEditor()
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
