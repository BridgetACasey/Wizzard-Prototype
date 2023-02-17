//@BridgetACasey

#include "wzpch.h"

#include "WinOSWindow.h"

#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/MouseEvent.h"
#include "wizzard/event/KeyEvent.h"

#include <glad/glad.h>

#include "platform/opengl/OpenGLContext.h"

namespace Wizzard
{
	static bool glfwInitialised = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		WIZ_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window * Window::Create(const WindowProps & props)
	{
		WIZ_PROFILE_FUNCTION();

		return new WinOSWindow(props);
	}

	WinOSWindow::WinOSWindow(const WindowProps& props)
	{
		WIZ_PROFILE_FUNCTION();

		Init(props);
	}

	WinOSWindow::~WinOSWindow()
	{
		WIZ_PROFILE_FUNCTION();

		Shutdown();
	}

	void WinOSWindow::OnUpdate()
	{
		WIZ_PROFILE_FUNCTION();

		glfwPollEvents();
		graphicsContext->SwapBuffers();
	}

	void WinOSWindow::SetVSync(bool enabled)
	{
		WIZ_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		windowData.vSync = enabled;
	}

	bool WinOSWindow::IsVSync() const
	{
		WIZ_PROFILE_FUNCTION();

		return windowData.vSync;
	}

	void WinOSWindow::Init(const WindowProps & props)
	{
		WIZ_PROFILE_FUNCTION();

		windowData.title = props.title;
		windowData.width = props.width;
		windowData.height = props.height;

		WIZ_INFO("Creating window {0} ({1}, {2})", windowData.title, windowData.width, windowData.height);

		if(!glfwInitialised)
		{
			//To do: GLFW Terminate on system shutdown

			int glfwStatus = glfwInit();

			WIZ_ASSERT(glfwStatus, "Could not initialise GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			glfwInitialised = true;
		}

		glfwWindow = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), windowData.title.c_str(), nullptr, nullptr);

		graphicsContext = new OpenGLContext(glfwWindow);
		graphicsContext->Init();

		glfwSetWindowUserPointer(glfwWindow, &windowData);
		SetVSync(true);


		// Set GLFW window event callbacks
		glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}

	void WinOSWindow::Shutdown()
	{
		WIZ_PROFILE_FUNCTION();

		glfwDestroyWindow(glfwWindow);

		if(glfwInitialised)
		glfwTerminate();
	}
}