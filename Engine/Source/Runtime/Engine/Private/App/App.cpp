// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "App/App.h"
#include "Engine/GameProject.h"
#include "System.h"


FApp::FApp()
	: m_IsRunning( true )
{
	ZeroMemory( m_Name, sizeof( m_Name ) );
}

FApp::~FApp()
{

}

void FApp::Startup()
{
	//PlatformSetup();
}

void FApp::Shutdown()
{
	//PlatformShutdown();
}

///*static*/ FApp* FApp::CreateApplication()
//{
//	return PlatformCreateApp();
//}
//
//void FApp::PumpMessageQueue()
//{
//	PlatformPumpMessages();
//
//}
//
///*static*/ FApp* FApp::PlatformCreateApp()
//{
//#if HE_WINDOWS_DESKTOP
//	return new FApp();
//
//#elif  HE_WINDOWS_UNIVERSAL
//
//	auto pApp = winrt::make<FApp>();
//	CoreApplication::Run( pApp );
//	return pApp.as<FApp>().get();
//#endif
//}
//
//bool FApp::PlatformSetup()
//{
//#if HE_WINDOWS_UNIVERSAL
//
//	return true;
//#endif
//}
//
//bool FApp::PlatformShutdown()
//{
//#if HE_WINDOWS_DESKTOP
//
//#elif  HE_WINDOWS_UNIVERSAL
//	CoreApplication::Exit();
//
//	return true;
//#endif
//}
//
//void FApp::PlatformPumpMessages()
//{
//#if HE_WINDOWS_DESKTOP
//	System::ProcessMessages();
//
//#elif HE_WINDOWS_UNIVERSAL
//	CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents( CoreProcessEventsOption::ProcessAllIfPresent );
//
//#endif
//}
//
//
////
//// Windows Desktop methods
////
//
//// None
//
//
////
//// Windows Universal methods
////
//
//#if HE_WINDOWS_UNIVERSAL
//
//void FApp::Initialize( CoreApplicationView const& ApplicationView )
//{
//	ApplicationView.Activated( { this, &FApp::OnActivated } );
//}
//
//void FApp::Uninitialize() noexcept
//{
//
//}
//
//void FApp::Run()
//{
//	while (IsRunning())
//	{
//
//	}
//}
//
//void FApp::Load( const winrt::hstring& ) noexcept
//{
//
//}
//
//void FApp::OnActivated( const CoreApplicationView& ApplicationView, const IActivatedEventArgs& Args )
//{
//
//}
//
//void FApp::SetWindow( CoreWindow const& Window )
//{
//	Window.Activate();
//}
//
//IFrameworkView FApp::CreateView()
//{
//	return *this;
//}
//
//#endif