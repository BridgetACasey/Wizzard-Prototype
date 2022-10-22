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

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> window;

		bool running = true;

		LayerStack layerStack;
	};

	Application* CreateApplication();	//To be defined in the client
}