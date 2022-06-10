//@BridgetACasey

#include "wzpch.h"
#include "WindowsWindow.h"

namespace Wizzard
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case WM_CHAR:
			if (wparam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
			}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);

		WNDCLASS windowClass = { 0 };

		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hInstance = NULL;
		windowClass.lpfnWndProc = WndProc;
		windowClass.lpszClassName = L"WIZZARD Window";
		windowClass.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClass(&windowClass))
			MessageBox(NULL, L"Could not register class", L"Error", MB_OK);

		HWND windowHandle = CreateWindow(L"WIZZARD Window",
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

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;
	}

	void WindowsWindow::onUpdate()
	{
		MSG messages;

		if(GetMessage(&messages, NULL, 0, 0) > 0)
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
	}

	void WindowsWindow::shutdown()
	{
	}

	void WindowsWindow::setVSync(bool enabled)
	{
	}

	bool WindowsWindow::isVSync() const
	{
		return false;
	}

	void* WindowsWindow::getNativeWindow() const
	{
		return nullptr;
	}
}