//@BridgetACasey

#pragma once

#include "wizzard/core/Window.h"
#include "wizzard/rendering/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Wizzard
{
	class WinOSWindow : public Window
	{
	public:
		WinOSWindow(const WindowProps& props);
		virtual ~WinOSWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return windowData.width; }
		unsigned int GetHeight() const override { return windowData.height; }

		void SetEventCallback(const EventCallbackFunction& callback) override { windowData.eventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		GLFWwindow* glfwWindow;
		GraphicsContext* graphicsContext;

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		void* GetNativeWindow() const override { return glfwWindow; }

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFunction eventCallback;
		};

		WindowData windowData;
	};
}