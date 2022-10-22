//@BridgetACasey

#include "wzpch.h"

#include "Window_WinGL.h"

#include "../../event/ApplicationEvent.h"
#include "../../event/MouseEvent.h"
#include "../../event/KeyEvent.h"

//Remove this after testing
#include <core/Log.h>

namespace Wizzard
{
	static bool glfwInitialised = false;

	Window * Window::Create(const WindowProps & props)
	{
		return new Window_WinGL(props);
	}

	Window_WinGL::Window_WinGL(const WindowProps& props)
	{
		Init(props);
	}

	Window_WinGL::~Window_WinGL()
	{
		Shutdown();
	}

	void Window_WinGL::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(glfwWindow);
	}

	void Window_WinGL::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		data.vSync = enabled;
	}

	bool Window_WinGL::IsVSync() const
	{
		return data.vSync;
	}

	void Window_WinGL::Init(const WindowProps & props)
	{
		//Converting title into appropriate string format for GLFW create window function
		std::wstring wTitle(props.title);

		data.title = std::string(wTitle.begin(), wTitle.end());
		data.width = props.width;
		data.height = props.height;

		WIZ_INFO("Creating window {0} ({1}, {2})", data.title, data.width, data.height);

		if(!glfwInitialised)
		{
			//To do: GLFW Terminate on system shutdown

			int initSuccess = glfwInit();
			WIZ_ASSERT(initSuccess, "Could not initialise GLFW!");

			glfwInitialised = true;
		}

		glfwWindow = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), data.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(glfwWindow);
		glfwSetWindowUserPointer(glfwWindow, &data);
		SetVSync(true);
	}

	void Window_WinGL::Shutdown()
	{
		glfwDestroyWindow(glfwWindow);
	}

	void* Window_WinGL::GetNativeWindow() const
	{
		return nullptr;
	}
}