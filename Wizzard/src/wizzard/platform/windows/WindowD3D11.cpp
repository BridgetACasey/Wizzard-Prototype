//@BridgetACasey

#include "wzpch.h"
#include "WindowD3D11.h"

#include "../../event/ApplicationEvent.h"
#include "../../event/MouseEvent.h"
#include "../../event/KeyEvent.h"

//Remove this after testing
#include <core/Log.h>

namespace Wizzard
{
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowD3D11(props);
	}

	WindowD3D11::WindowD3D11(const WindowProps& props)
	{
		init(props);
	}

	WindowD3D11::~WindowD3D11()
	{
		shutdown();
	}

	LRESULT WindowD3D11::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		WindowD3D11* me = reinterpret_cast<WindowD3D11*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (me)
		{
			return me->realWndProc(hwnd, message, wparam, lparam);
		}

		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	//Setting callback events
	LRESULT CALLBACK WindowD3D11::realWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		//Using chained else-if instead of switch due to initialising event calls
		if (message == WM_CHAR)
		{
			if (wparam == VK_ESCAPE)
				DestroyWindow(hwnd);
		}
		else if (message == WM_DESTROY)
		{
			PostQuitMessage(0);
		}
		else if (message == WM_SIZE)
		{
			RECT windowRect;

			if (GetWindowRect(hwnd, &windowRect))
			{
				int width = windowRect.right - windowRect.left;
				int height = windowRect.bottom - windowRect.top;

				WindowResizeEvent event(width, height);
				WIZZARD_TRACE(event);
				data.eventCallback(event);
			}
		}
		else if (message == WM_KEYDOWN)
		{
			KeyPressedEvent event(wparam, 0);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else if (message == WM_KEYUP)
		{
			KeyReleasedEvent event(wparam);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
		{
			MouseButtonPressedEvent event(wparam);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
		{
			MouseButtonReleasedEvent event(wparam);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else if (message == WM_MOUSEMOVE)
		{
			int xPos = GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);

			MouseMovedEvent event(xPos, yPos);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else if (message == WM_MOUSEWHEEL)
		{
			int xPos = GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);

			MouseScrolledEvent event(xPos, yPos);
			WIZZARD_TRACE(event);
			data.eventCallback(event);
		}
		else
			return DefWindowProc(hwnd, message, wparam, lparam);
	}

	void WindowD3D11::init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		//Setting up the window
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hInstance = NULL;
		windowClass.lpfnWndProc = WndProc;
		windowClass.lpszClassName = L"WIZZARD Window";
		windowClass.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClass(&windowClass))
			MessageBox(NULL, L"Could not register class", L"Error", MB_OK);

			windowHandle = CreateWindow(L"WIZZARD Window",
			data.title,
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			data.width,
			data.height,
			NULL,
			NULL,
			NULL,
			NULL);

		ShowWindow(windowHandle, SW_RESTORE);
	}

	void WindowD3D11::onUpdate()
	{
		MSG messages;

		if(GetMessage(&messages, NULL, 0, 0) > 0)
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}

	void WindowD3D11::shutdown()
	{
	}

	void WindowD3D11::setVSync(bool enabled)
	{
	}

	bool WindowD3D11::isVSync() const
	{
		return false;
	}

	void* WindowD3D11::getNativeWindow() const
	{
		return nullptr;
	}
}