//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "wizzard/event/Event.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/ui/ImGuiLayer.h"
#include "wizzard/base/Timestep.h"

namespace Wizzard
{
	class Application
	{
	public:
		Application(const std::string& name = "Wizzard Application");
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		ImGuiLayer* GetImGuiLayer() { return imguiLayer; }

		Window& GetWindow() { return *window; }

		static Application& Get() { return *appInstance; }

	protected:
		virtual void OnApplicationInit();
		virtual void OnApplicationShutdown();

	private:
		bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
		bool OnWindowResize(WindowResizeEvent& windowResizeEvent);

		//Probably temporary, take another look at this later
		bool OnUIElementFocus(UIElementFocusEvent& uiElementFocusEvent);
		bool OnUIElementSelected(UIElementSelectedEvent& uiElementSelectedEvent);

		std::unique_ptr<Window> window;

		bool running = true;
		bool minimized = false;

		LayerStack layerStack;

		static Application* appInstance;

		ImGuiLayer* imguiLayer;

		float lastFrameTime = 0.0f;
	};

	Application* CreateApplication();	//To be defined in the client
}