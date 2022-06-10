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

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

	private:
		bool onWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> window;

		bool running = true;

		LayerStack layerStack;
	};

	Application* createApplication();	//To be defined in the client
}