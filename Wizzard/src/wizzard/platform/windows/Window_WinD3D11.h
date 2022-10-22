//@BridgetACasey

#pragma once

#include "core/Window.h"

namespace Wizzard
{
	class Window_WinD3D11 : public Window
	{
	public:
		Window_WinD3D11(const WindowProps& props);
		virtual ~Window_WinD3D11();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return data.width; }
		unsigned int GetHeight() const override { return data.height; }

		void SetEventCallback(const EventCallbackFunction& callback) override
		{
			data.eventCallback = callback;
			//Setting pointer to this window class so we can access the correct WndProc
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);
		}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		LRESULT CALLBACK RealWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		void* GetNativeWindow() const override;

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