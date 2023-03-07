//@BridgetACasey

#pragma once

#include "Event.h"

namespace Wizzard
{
	class UIEvent : public Event
	{
	public:
		unsigned int GetElementID() { return elementID; }

		EVENT_CLASS_CATEGORY(EventCategoryEditor | EventCategoryInput)

	protected:
		UIEvent(unsigned int elementID) : elementID(elementID) {}

		unsigned int elementID;
	};

	class UIElementFocusEvent : public UIEvent
	{
	public:
		UIElementFocusEvent(unsigned int elementID, bool focus) : UIEvent(elementID), isFocused(focus){}

		bool GetIsFocused() const { return isFocused; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIElementFocusEvent: ImGui ID - " << elementID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIElementFocus)

	protected:
		bool isFocused;
	};

	class UIElementSelectedEvent : public UIEvent
	{
	public:
		UIElementSelectedEvent(unsigned int elementID, bool select) : UIEvent(elementID), isSelected(select){}

		bool GetIsSelected() const { return isSelected; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIElementSelectedEvent: ImGui ID - " << elementID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIElementSelected)

	protected:
		bool isSelected;
	};

	class UIWindowFocusEvent : public UIEvent
	{
	public:
		UIWindowFocusEvent(unsigned int elementID, bool focus) : UIEvent(elementID), isFocused(focus) {}

		bool GetIsFocused() const { return isFocused; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIWindowFocusEvent: ImGui Window ID - " << elementID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIWindowFocus)

	protected:
		bool isFocused;
	};
}