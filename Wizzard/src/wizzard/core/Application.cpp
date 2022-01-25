//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "event/ApplicationEvent.h"
#include "Log.h"

namespace Wizzard
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::run()
	{
		WindowResizeEvent resizeEvent(1600, 900);

		if (resizeEvent.isInCategory(EventCategoryApplication))
		{
			LOG_WIZZARD_TRACE(resizeEvent);
		}
		
		if (resizeEvent.isInCategory(EventCategoryInput))
		{
			LOG_WIZZARD_TRACE(resizeEvent);
		}

		while (true);
	}
}