//@BridgetACasey

#pragma once

#include "core/Core.h"

namespace Wizzard
{
	//For future versions: attempt to buffer events into an event bus and process them during the event phase of application updating, instead of handling them instantly

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
	virtual EventType getEventType() const override { return getStaticType(); }\
	virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getEventCategoryFlags() const override { return category };

	enum class EventType
	{
		None,
		WindowClosed,
		WindowResized,
		WindowFocused,
		WindowNotFocused,
		WindowMoved,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseMoved,
		MouseScrolled,
		MouseEntered,
		MouseExited,
		MouseButtonPressed,
		MouseButtonReleased
	};

	enum EventCategory
	{
		None,
		EventCategoryWindow = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class WIZZARD_API Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType getEventType() const = 0;
		virtual const char* getEventName() const = 0;
		virtual int getEventCategoryFlags() const = 0;

		inline bool isInCategory(EventCategory category)
		{
			return getEventCategoryFlags() & category;
		}

		bool isHandled = false;
	};
}