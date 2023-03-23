//@BridgetACasey

#pragma once

#include "wizzard/common/UUID.h"

namespace Wizzard
{
	class EntitySelection
	{
	public:
		static void SelectEntity(UUID entityID);
		static bool IsSelected(UUID entityID);
		static void DeselectEntity(UUID entityID);
		static void DeselectAll();
		static std::vector<UUID>& GetSelections() { return selectedEnityIDs; }

	private:
		static std::vector<UUID> selectedEnityIDs;
	};
}