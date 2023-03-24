//@BridgetACasey

#pragma once

#include <vector>
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class EntitySelection
	{
	public:
		static void SelectEntity(Entity entity);
		static bool IsSelected(Entity entity);
		static void DeselectEntity(Entity entity);
		static void DeselectAll();
		static std::vector<Entity>& GetSelections() { return selectedEntities; }

		static void ToggleMultiSelectMode() { multiSelectMode = !multiSelectMode; }
		static bool IsMultiSelect() { return multiSelectMode; }

	private:
		inline static std::vector<Entity> selectedEntities;
		inline static bool multiSelectMode = false;
	};
}