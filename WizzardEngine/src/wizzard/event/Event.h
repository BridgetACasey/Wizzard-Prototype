//@BridgetACasey

#pragma once

#include "wizzard/wzpch.h"
#include "wizzard/base/Base.h"

namespace Wizzard
{
	//TODO: for future versions, attempt to buffer events into an event bus and process them during the event phase of application updating, instead of handling them instantly

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char* GetEventName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategoryFlags() const override { return category; }

	enum class EventType
	{
		None = 0,

		WindowClose,
		WindowResize,
		WindowFocus,
		WindowNotFocused,
		WindowMoved,

		ApplicationStartup,
		ApplicationTick,
		ApplicationUpdate,
		ApplicationRender,
		ApplicationShutdown,

		KeyPressed,
		KeyDown,
		KeyReleased,
		KeyTyped,

		MouseMoved,
		MouseScrolled,
		MouseEntered,
		MouseExited,
		MouseButtonPressed,
		MouseButtonDown,
		MouseButtonReleased,

		UIWindowHovered,
		UIWindowFocus,
		UIElementHovered,
		UIElementSelected,

		ViewportSelectionHovered,
		ViewportSelectionChanged,

		SceneBeginPlay,
		ScenePreBeginPlay,
		ScenePostBeginPlay,
		ScenePreEndPlay,
		ScenePostEndPlay,
		SceneEndPlay,

		AudioTrackStarted,
		AudioTrackEnded,
		ScreenReaderMessageStarted,
		ScreenReaderMessageEnded
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryEditor = BIT(5),
		EventCategoryScene = BIT(6),
		EventCategoryAudio = BIT(7)
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		[[nodiscard]] virtual EventType GetEventType() const = 0;
		[[nodiscard]] virtual const char* GetEventName() const = 0;
		[[nodiscard]] virtual int GetEventCategoryFlags() const = 0;
		[[nodiscard]] virtual std::string ToString() const { return GetEventName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetEventCategoryFlags() & category;
		}

		bool isHandled = false;
	};

	inline std::ostream& operator<<(std::ostream& oStream, const Event& event)
	{
		return oStream << event.ToString();
	}
}