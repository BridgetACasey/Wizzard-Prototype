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

		static void ToggleMultiSelectMode() { multiSelectMode = !multiSelectMode; }
		static bool IsMultiSelect() { return multiSelectMode; }

		static void DescribeSelectionsIsolated();
		static void DescribeSelectionsRelational();

		static std::vector<Entity>& GetSelections() { return selectedEntities; }

	private:
		inline static std::vector<Entity> selectedEntities;
		inline static bool multiSelectMode = false;
	};
}