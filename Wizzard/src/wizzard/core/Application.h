//@BridgetACasey

#pragma once

#include "Core.h"
#include "Window.h"

namespace Wizzard
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

	private:
		std::unique_ptr<Window> window;

		bool running = true;
	};

	Application* createApplication();	//To be defined in the client
}