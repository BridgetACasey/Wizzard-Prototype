//@BridgetACasey

#pragma once

#include "../event/Event.h"

namespace Wizzard
{
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Wizzard", uint32_t width = 1600, uint32_t height = 900)
			: title(title), width(width), height(height){}
	};

	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}