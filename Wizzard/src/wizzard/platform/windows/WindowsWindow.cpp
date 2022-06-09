//@BridgetACasey

#include "wzpch.h"
#include "WindowsWindow.h"

namespace Wizzard
{
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::onUpdate()
	{
	}

	void WindowsWindow::setVSync(bool enabled)
	{
	}

	bool WindowsWindow::isVSync() const
	{
		return false;
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;
	}

	void WindowsWindow::shutdown()
	{
	}

	void* WindowsWindow::getNativeWindow() const
	{
		return nullptr;
	}
}