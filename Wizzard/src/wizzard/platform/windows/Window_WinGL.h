//@BridgetACasey

//A window rendered with Modern OpenGL. To be implemented later.

#pragma once

#include "core/Window.h"

#include <GLFW/glfw3.h>

namespace Wizzard
{
	class Window_WinGL : public Window
	{
	public:
		Window_WinGL(const WindowProps& props);
		virtual ~Window_WinGL();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return data.width; }
		unsigned int GetHeight() const override { return data.height; }

		void SetEventCallback(const EventCallbackFunction& callback) override { data.eventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		GLFWwindow* glfwWindow;

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		void* GetNativeWindow() const override;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFunction eventCallback;
		};

		WindowData data;
	};
}