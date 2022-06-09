//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "event/ApplicationEvent.h"
#include "Log.h"

namespace Wizzard
{
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
			window->onUpdate();
		}
	}
}