//@BridgetACasey

#pragma once

#include "Event.h"

namespace Wizzard
{
	class AudioEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryAudio)
	};

	class ScreenReaderMessageStartedEvent : public AudioEvent
	{
	public:
		ScreenReaderMessageStartedEvent(std::string& message) : message(message){}

		EVENT_CLASS_TYPE(ScreenReaderMessageStarted)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScreenReaderMessageStartedEvent: " << message;
			return ss.str();
		}

		const std::string& GetSRMessage() { return message; }

	protected:
		std::string message;
	};

	class ScreenReaderMessageEndedEvent : public AudioEvent
	{
	public:
		ScreenReaderMessageEndedEvent(std::string& message) : message(message){}

		EVENT_CLASS_TYPE(ScreenReaderMessageEnded)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScreenReaderMessageEndedEvent: " << message;
			return ss.str();
		}

		const std::string& GetSRMessage() { return message; }

	protected:
		std::string message;
	};
}