// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "App/FApp.h"
#include "CommandLine.h"
#include "RenderContext.h"
#include "RendererInitializer.h"
#include "Engine/ViewportContext.h"
#include "Engine/FrameTimeManager.h"
#include "AssetRegistry/AssetDatabase.h"
#include "World/HWorld.h"
#include "Engine/GameProject.h"


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
	ViewportContext& GetClientViewport();
	
	/*
		Returns the time between frame buffer flips.
	*/
	float GetDeltaTime() const;

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
	bool				m_IsInitialized;
	bool				m_IsEditorPresent;
	bool				m_IsPlayingInEditor;
	FrameTimeManager	m_FrameTimeManager;
	ViewportContext		m_MainViewPort;
	RendererInitializer	m_RenderContextInitializer;
	RenderContext		m_RenderContext;
	FApp				m_Application;
	FGameProject		m_GameProject;
	AssetDatabase		m_AssetDatabase;
	HWorld				m_GameWorld;

};

extern HEngine* GEngine;
extern ThreadPool* GThreadPool;


//
// Inline function implementations
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
	return (float)m_FrameTimeManager.GetFrameTime();
}

inline bool HEngine::GetIsEditorPresent()
{
#if HE_SHIPPING
	return false;
#else
	return m_IsEditorPresent;
#endif
}

inline bool HEngine::IsPlayingInEditor()
{
#if HE_SHIPPING
	return true;
#else
	return m_IsPlayingInEditor;
#endif
}

inline void HEngine::SetIsPlayingInEditor( bool IsPlaying )
{
#if HE_SHIPPING
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
