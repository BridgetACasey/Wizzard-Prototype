//@BridgetACasey

#pragma once

#include "Event.h"
#include "wizzard/scene/Entity.h"
#include "wizzard/common/UUID.h"

namespace Wizzard
{
	class EditorEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryEditor)

	protected:
		EditorEvent() = default;
	};

	class ViewportSelectionHoveredEvent : public EditorEvent
	{
	public:
		ViewportSelectionHoveredEvent(Entity handle, UUID selectionID, bool selected) :
			selectionContext(handle), selectionID(selectionID), selected(selected) {}

		Entity GetSelectionContext() const { return selectionContext; }
		UUID GetSelectionID() const { return selectionID; }
		bool IsSelected() const { return selected; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ViewportSelectionHoveredEvent: Selection(" << selectionID << "), " << selected;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEditor | EventCategoryInput)
			EVENT_CLASS_TYPE(ViewportSelectionHovered)

	protected:
		Entity selectionContext;
		UUID selectionID;
		bool selected;
	};

	class ViewportSelectionChangedEvent : public EditorEvent
	{
	public:
		ViewportSelectionChangedEvent(Entity handle, UUID selectionID, bool selected) :
		selectionContext(handle), selectionID(selectionID), selected(selected){}

		Entity GetSelectionContext() const { return selectionContext; }
		UUID GetSelectionID() const { return selectionID; }
		bool IsSelected() const { return selected; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ViewportSelectionChangedEvent: Selection(" << selectionID << "), " << selected;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEditor | EventCategoryInput)
		EVENT_CLASS_TYPE(ViewportSelectionChanged)

	protected:
		Entity selectionContext;
		UUID selectionID;
		bool selected;
	};
}