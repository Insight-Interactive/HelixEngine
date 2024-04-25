// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/HelixEngine.h"

WChar* GCommandLine;

class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView>
{
public:
	ViewProvider() noexcept
	{
	}

	// IFrameworkView methods
	void Initialize( const CoreApplicationView& applicationView )
	{
		applicationView.Activated( { this, &ViewProvider::OnActivated } );

	}

	void Uninitialize() noexcept
	{
	}

	void SetWindow( const CoreWindow& window )
	{
		/* winrt::Windows::System::VirtualKey k = winrt::Windows::System::VirtualKey::A;
		 auto state = window.GetAsyncKeyState(k);

		 if (state == winrt::Windows::UI::Core::CoreVirtualKeyStates::Down)
		 {

		 }*/

		window.Closed( { this, &ViewProvider::OnWindowClosed } );
		auto dispatcher = CoreWindow::GetForCurrentThread().Dispatcher();

		dispatcher.AcceleratorKeyActivated( { this, &ViewProvider::OnAcceleratorKeyActivated } );

		auto navigation = SystemNavigationManager::GetForCurrentView();

		// UWP on Xbox One triggers a back request whenever the B button is pressed
		// which can result in the app being suspended if unhandled
		navigation.BackRequested( []( const winrt::Windows::Foundation::IInspectable&, const BackRequestedEventArgs& args )
			{
				args.Handled( true );
			} );

		DisplayInformation::DisplayContentsInvalidated( { this, &ViewProvider::OnDisplayContentsInvalidated } );

	}

	void Load( const winrt::hstring& ) noexcept
	{
	}

	void Run()
	{
		GuardedMain( GCommandLine );
	}

protected:
	void OnWindowClosed( const CoreWindow& Sender, const CoreWindowEventArgs& Args )
	{

	}
	void OnAppSuspending( const IInspectable& Sender, const SuspendingEventArgs& Args )
	{
		OutputDebugString( TEXT( "Suspending\n" ) );
	}

	void OnAppResuming( const IInspectable& Sender, const IInspectable& Args )
	{
		OutputDebugString( TEXT( "Resuming\n" ) );
	}
	// Event handlers
	void OnActivated( const CoreApplicationView& ApplicationView, const IActivatedEventArgs& Args )
	{
		if (Args.Kind() == ActivationKind::Launch)
		{
			auto launchArgs = (const LaunchActivatedEventArgs*)(&Args);

			if (launchArgs->PrelaunchActivated())
			{
				// Opt-out of Prelaunch
				CoreApplication::Exit();
				return;
			}
		}
		CoreApplication::Suspending( { this, &ViewProvider::OnAppSuspending } );
		CoreApplication::Resuming( { this, &ViewProvider::OnAppResuming } );

		// Change to ApplicationViewWindowingMode::FullScreen to default to full screen or PreferredLaunchViewSize for windowed
		ApplicationView::PreferredLaunchWindowingMode( ApplicationViewWindowingMode::PreferredLaunchViewSize );

		auto desiredSize = Size( 1920.f, 1080.f );

		ApplicationView::PreferredLaunchViewSize( desiredSize );

		auto view = ApplicationView::GetForCurrentView();

		auto minSize = Size( 320, 200 );

		view.SetPreferredMinSize( minSize );

		CoreWindow::GetForCurrentThread().Activate();

		view.FullScreenSystemOverlayMode( FullScreenSystemOverlayMode::Minimal );

		view.TryResizeView( desiredSize );
	}

	void OnAcceleratorKeyActivated( const CoreDispatcher&, const AcceleratorKeyEventArgs& args )
	{
		//if (args.EventType() == CoreAcceleratorKeyEventType::SystemKeyDown
		//	&& args.VirtualKey() == VirtualKey::Enter
		//	&& args.KeyStatus().IsMenuKeyDown
		//	&& !args.KeyStatus().WasKeyDown)
		//{
		//	// Implements the classic ALT+ENTER fullscreen toggle
		//	auto view = ApplicationView::GetForCurrentView();

		//	if (view.IsFullScreenMode())
		//		view.ExitFullScreenMode();
		//	else
		//		view.TryEnterFullScreenMode();

		//	args.Handled( true );
		//}
	}

	void OnDisplayContentsInvalidated( const DisplayInformation& Sender, const IInspectable& Args )
	{
		//m_game->ValidateDevice();
	}

private:

};


class ViewProviderFactory : public winrt::implements<ViewProviderFactory, IFrameworkViewSource>
{
public:
	IFrameworkView CreateView()
	{
		return winrt::make<ViewProvider>();
	}
};

HE_ENTRY_POINT()
{
	GCommandLine = CmdLine;

	auto viewProviderFactory = winrt::make<ViewProviderFactory>();
	CoreApplication::Run( viewProviderFactory );

	return 0;
}
