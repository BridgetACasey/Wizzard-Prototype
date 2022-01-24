//@BridgetACasey

#pragma once

#include "Event.h"

namespace Wizzard
{
	class EventHandler
	{
	public:
		EventHandler(Event& event) : event(event) {}

		template<typename T, typename F>

		bool dispatchEvent(const F& func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.isHandled |= func(static_cast<T&>(event));

				return true;
			}

			return false;
		}

	private:
		Event& event;
	};
}