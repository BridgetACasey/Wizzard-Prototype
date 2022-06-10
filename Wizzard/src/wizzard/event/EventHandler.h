//@BridgetACasey

#pragma once

#include "../wzpch.h"
#include "Event.h"

namespace Wizzard
{
	class EventHandler
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventHandler(Event& event) : event(event) {}

		template<typename T>
		bool dispatchEvent(EventFn<T> func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.isHandled = func(*(T*)&event);
				return true;
			}

			return false;
		}

	private:
		Event& event;
	};
}