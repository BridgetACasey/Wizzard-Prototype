//@BridgetACasey

#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "../event/Event.h"
#include "../event/ApplicationEvent.h"

namespace Wizzard
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *window; }

		static Application& Get() { return *appInstance; }

	private:
		bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
		
		std::unique_ptr<Window> window;

		bool running = true;

		LayerStack layerStack;

		static Application* appInstance;
	};

	Application* CreateApplication();	//To be defined in the client
}