//@BridgetACasey

#pragma once

#include "Event.h"
#include "wizzard/core/KeyCode.h"

namespace Wizzard
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(const KeyCode keyCode) : keyCode(keyCode) {}

		KeyCode keyCode;
	};

	//TODO: Rework key events to include input option for a key being pressed only once e.g. KeyPressedEvent() and KeyHeldEvent()
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const uint16_t repCount) : KeyEvent(keycode), repeatCount(repCount) {}

		uint16_t GetRepeatCount() const { return repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << (char)keyCode << " (ASCII Value: " << keyCode << ") (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		uint16_t repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << (char)keyCode << " (ASCII Value: " << keyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}