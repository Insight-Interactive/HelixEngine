// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/EngineDefines.h"
#include "TSingleton.h"


const uint32 kMaxAppNameLength = 64;

class FApp :  public TSingleton<FApp>
{
	HE_DECL_NON_COPYABLE( FApp )
public:
	FApp();
	~FApp();

	void Startup();
	void Shutdown();
	
	/*
		Returns the name of the application that is currently running.
	*/
	const TChar* GetName() const;

	/*
		Set the name of the application.
	*/
	void SetName( const TChar* AppName );

	/*
		Returns true if the application is running, false if not.
	*/
	bool IsRunning() const;

	/*
		Request the application to terminate. Shutting down the engine and its subsystmes.
	*/
	void RequestShutdown();

//protected:
//	static FApp* CreateApplication();
//
//	void PumpMessageQueue();
//
//private:
//	static FApp* PlatformCreateApp();
//
//	/*
//		Setup any platform dependent resources.
//	*/
//	bool PlatformSetup();
//	
//	/*
//		Shutdown any platform dependent resources.
//	*/
//	bool PlatformShutdown();
//
//	/*
//	
//	*/
//	void PlatformPumpMessages();

protected:
	TChar m_Name[kMaxAppNameLength];
	bool m_IsRunning;

//	// Platform 
//
//#if HE_WINDOWS_UNIVERSAL
//public:
//	// winrt::IFrameworkView interface implementations
//	//
//	void Initialize( CoreApplicationView const& ApplicationView );
//	void Uninitialize() noexcept;
//	void Run();
//	void Load( const winrt::hstring& ) noexcept;
//	void OnActivated( const CoreApplicationView& ApplicationView, const IActivatedEventArgs& Args );
//	void SetWindow( CoreWindow const& Window );
//	IFrameworkView CreateView();
//
//
//#endif
};


//
// Inline function implementations
//

// App
//

inline const TChar* FApp::GetName() const 
{
	HE_ASSERT( m_Name != NULL );
	return m_Name;
}

inline void FApp::SetName( const TChar* AppName )
{
	HE_ASSERT( AppName != NULL );
	TCharStrCpy( m_Name, AppName );
}

inline bool FApp::IsRunning() const
{
	return m_IsRunning;
}

inline void FApp::RequestShutdown()
{
	m_IsRunning = false;
}
