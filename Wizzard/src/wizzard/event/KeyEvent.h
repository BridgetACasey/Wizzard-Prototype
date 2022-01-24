//@BridgetACasey

#pragma once

#include "Event.h"
#include "core/KeyCode.h"

namespace Wizzard
{
	class WIZZARD_API KeyEvent : public Event
	{
	public:
		KeyCode getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(const KeyCode keyCode) : m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{

	};
}