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

	class UIElementHoveredEvent : public UIEvent
	{
	public:
		UIElementHoveredEvent(unsigned int elementID, bool hovered) : UIEvent(elementID), isHovered(hovered){}

		bool GetIsHovered() const { return isHovered; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "UIElementHoveredEvent: ImGui ID - " << elementID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(UIElementHovered)

	protected:
		bool isHovered;
	};

	class UIElementSelectedEvent : public UIEvent
	{
	public:
		UIElementSelectedEvent(unsigned int elementID, bool selected) : UIEvent(elementID), isSelected(selected){}

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
}