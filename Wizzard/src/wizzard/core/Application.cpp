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
	}

	Application::~Application()
	{
		
	}

	void Application::run()
	{
		WindowResizeEvent resizeEvent(1600, 900);

		if (resizeEvent.isInCategory(EventCategoryApplication))
		{
			WIZZARD_TRACE(resizeEvent);
		}
		
		if (resizeEvent.isInCategory(EventCategoryInput))
		{
			WIZZARD_TRACE(resizeEvent);
		}

		while (running)
		{
			for (Layer* layer : layerStack)
			{
				layer->onUpdate();
			}

			window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		EventHandler eventHandler(e);

		eventHandler.dispatchEvent<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->onEvent(e);

			if (e.isHandled)
				break;
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		layerStack.pushOverlay(overlay);
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}