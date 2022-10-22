//@BridgetACasey

#pragma once

#include "core/Window.h"

namespace Wizzard
{
	class WindowD3D11 : public Window
	{
	public:
		WindowD3D11(const WindowProps& props);
		virtual ~WindowD3D11();

		void onUpdate() override;

		inline unsigned int getWidth() const override { return data.width; }
		inline unsigned int getHeight() const override { return data.height; }

		inline void setEventCallback(const EventCallbackFunction& callback) override
		{
			data.eventCallback = callback;
			//Setting pointer to this window class so we can access the correct WndProc
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);
		}

		void setVSync(bool enabled) override;
		bool isVSync() const override;

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		LRESULT CALLBACK realWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

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

		WNDCLASS windowClass = { 0 };
		HWND windowHandle;

		bool windowptr = false;
	};
}