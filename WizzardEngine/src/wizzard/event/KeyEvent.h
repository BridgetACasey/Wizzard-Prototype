//@BridgetACasey

#pragma once

#include "Event.h"
#include "wizzard/input/KeyCode.h"

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

	//TODO: Rework key events to include input option for a key being pressed only once e.g. KeyPressedEvent() and KeyDownEvent()
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << (char)keyCode << " (ASCII Value: " << keyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyDownEvent: " << (char)keyCode << " (ASCII Value: " << keyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyDown)
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