//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "../event/ApplicationEvent.h"
#include "../event/EventHandler.h"
#include "Log.h"

namespace Wizzard
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (running)
		{
			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventHandler eventHandler(e);

		eventHandler.HandleEvent<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->OnEvent(e);

			if (e.isHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}