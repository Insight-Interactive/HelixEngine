// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Window.h"
#include "System.h"
#include "Engine/Engine.h"

#include "RenderContextFactoryD3D12.h"



Window::Window()
	: m_WindowMode(WM_Windowed)
	, m_OnWindowClosedCallback(NULL)
{
}

Window::~Window()
{
	Destroy();
}

void Window::Create(const Window::Description& Desc)
{
	if (IsValid())
	{
		HE_LOG(Warning, TEXT("Trying to initialize a window that has already been created."));
		return;
	}

	m_Desc = Desc;

	static uint32 WindowInstanceCount = 0;

	PrintBuffer(m_WindowClassName, TEXT("WinClass-%i"), WindowInstanceCount);

	HE_LOG(Log, TEXT("Registering Windows Desktop window with title: %s (Class: %s)"), Desc.Title, m_WindowClassName);



	if (m_Desc.bHasTitleBar)
		SetTitle(m_Desc.Title);

	if (Desc.bShowImmediate)
		Show();
	
	CreateSwapChain();

	WindowInstanceCount++;
}

void Window::Destroy()
{
	if (IsValid())
	{


		if (m_OnWindowClosedCallback != NULL)
			m_OnWindowClosedCallback();
	}
}

bool Window::IsValid()
{
}

bool Window::IsVisible()
{
}

void Window::Show()
{
}

void Window::Hide()
{
}

bool Window::HasFocus()
{

}

void Window::SetFocus()
{

}

void Window::Maximize()
{

}

void Window::Minimize()
{

}

void Window::PresentOneFrame()
{
	m_pSwapChain->SwapBuffers();
}

bool Window::SetTitle(const TChar* NewTitle)
{
	if (!m_Desc.bHasTitleBar)
		HE_LOG(Warning, TEXT("Trying to set a title for a window that has no title. Attempting to override..."));

}

IColorBuffer* Window::GetRenderSurface()
{
	return m_pSwapChain->GetColorBufferForCurrentFrame();
}

void* Window::GetNativeWindow()
{
}

void Window::SetParent(Window* pParent)
{
}

void Window::OnWindowModeChanged()
{

	switch (GetWindowMode())
	{
	case EWindowMode::WM_FullScreen:
	{
		//
		// Bring us into fullscreen mode.
		//


		HE_LOG(Log, TEXT("Entering fullscreen mode."));

		m_pSwapChain->ToggleFullScreen(!m_pSwapChain->GetIsFullScreenEnabled());
	}
	break;
	case EWindowMode::WM_Windowed:
	{

		HE_LOG(Log, TEXT("Exiting fullscreen mode."));
	}
	break;
	}
}

void Window::CreateSwapChain()
{
	RendererInitializer::GetInstance()->InitializeSwapChain(&m_pSwapChain, GetNativeWindow(), GetWidth(), GetHeight());
}


//
// Windows Universal platform
//

