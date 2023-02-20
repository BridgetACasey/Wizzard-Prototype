//@BridgetACasey

#pragma once

#include "Event.h"

namespace Wizzard
{
	class UIElementFocusEvent : public Event
	{
	public:
		UIElementFocusEvent(bool focus) : isFocused(focus){}

		bool GetIsFocused() const { return isFocused; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIElementFocusEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIElementFocus)

	protected:
		bool isFocused;
	};

	class UIElementSelectedEvent : public Event
	{
	public:
		UIElementSelectedEvent(bool select) : isSelected(select){}

		bool GetIsSelected() const { return isSelected; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIElementSelectedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIElementSelected)

	protected:
		bool isSelected;
	};
}