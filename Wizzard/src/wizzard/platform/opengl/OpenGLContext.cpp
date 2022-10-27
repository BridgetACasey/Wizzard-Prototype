//@BridgetACasey

#include "wzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Wizzard
{
	OpenGLContext::OpenGLContext(GLFWwindow* wndHandle) : windowHandle(wndHandle)
	{
		WIZ_ASSERT(wndHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		WIZ_ASSERT(status, "Failed to initialise Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}
}