//@BridgetACasey

#pragma once

#include "Event.h"
#include "wizzard/scene/Entity.h"
#include "wizzard/common/UUID.h"

namespace Wizzard
{
	class SceneEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryScene)

	protected:
		SceneEvent() = default;
	};

	class ViewportSelectionChangedEvent : public SceneEvent
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

		EVENT_CLASS_CATEGORY(EventCategoryScene | EventCategoryInput)
		EVENT_CLASS_TYPE(ViewportSelectionChanged)

	protected:
		Entity selectionContext;
		UUID selectionID;
		bool selected;
	};
}