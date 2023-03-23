//@BridgetACasey

#pragma once

#include "Event.h"

namespace Wizzard
{
	template<typename TEvent>
	class MulticastDelegate
	{
	public:
		MulticastDelegate() = default;

		void Bind(std::function<bool(TEvent&)> func)
		{
			eventFuncs.push_back(func);
		}

		bool TriggerAll(Event& targetEvent)
		{
			if (targetEvent.GetEventType() == TEvent::GetStaticType())
			{
				handledAll = false;
				for (auto func : eventFuncs)
				{
					handledAll |= func(*(TEvent*)&targetEvent);
				}
				targetEvent.isHandled = handledAll;
			}

			return handledAll;
		}

		void Clear()
		{
			eventFuncs.clear();
		}

		bool HasHandledAll() const { return handledAll; }

	private:
		std::list<std::function<bool(TEvent&)>> eventFuncs;
		bool handledAll = false;
	};
}