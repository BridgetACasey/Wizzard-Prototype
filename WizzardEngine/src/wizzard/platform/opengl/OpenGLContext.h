//@BridgetACasey

#pragma once

#include "rendering/GraphicsContext.h"

struct GLFWwindow;

namespace Wizzard
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* wndHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}