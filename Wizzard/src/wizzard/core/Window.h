//@BridgetACasey

#pragma once

#include "../wzpch.h"

#include "Core.h"
#include "../event/Event.h"

namespace Wizzard
{
	struct WindowProps
	{
		wchar_t* title;
		uint32_t width;
		uint32_t height;

		WindowProps(wchar_t* title = L"WIZZARD", uint32_t width = 1920, uint32_t height = 1080)
			: title(title), width(width), height(height){}
	};

	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}