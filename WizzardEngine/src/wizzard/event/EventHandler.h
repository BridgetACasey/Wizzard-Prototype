//@BridgetACasey

#pragma once

#include "wizzard/wzpch.h"
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
		bool HandleEvent(EventFn<T> func)
		{
			if (event.GetEventType() == T::GetStaticType())
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