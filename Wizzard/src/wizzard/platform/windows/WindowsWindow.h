//@BridgetACasey

#pragma once

#include "core/Window.h"

namespace Wizzard
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		inline unsigned int getWidth() const override { return data.width; }
		inline unsigned int getHeight() const override { return data.height; }

		inline void setEventCallback(const EventCallbackFunction& callback) override { data.eventCallback = callback; }

		void setVSync(bool enabled) override;
		bool isVSync() const override;

	private:

		virtual void init(const WindowProps& props);
		virtual void shutdown();
		virtual void* getNativeWindow() const override;

		struct WindowData
		{
			wchar_t* title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFunction eventCallback;
		};

		WindowData data;
	};
}