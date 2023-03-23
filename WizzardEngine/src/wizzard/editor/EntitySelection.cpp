//@BridgetACasey

#include "wzpch.h"

#include "EntitySelection.h"

namespace Wizzard
{
	void EntitySelection::SelectEntity(UUID entityID)
	{
		if (IsSelected(entityID))
			return;

		selectedEnityIDs.push_back(entityID);
	}

	bool EntitySelection::IsSelected(UUID entityID)
	{
		return std::find(selectedEnityIDs.begin(), selectedEnityIDs.end(), entityID) != selectedEnityIDs.end();
	}

	void EntitySelection::DeselectEntity(UUID entityID)
	{
		auto itr = std::find(selectedEnityIDs.begin(), selectedEnityIDs.end(), entityID);

		if (itr == selectedEnityIDs.end())
			return;

		selectedEnityIDs.erase(itr);
	}

	void EntitySelection::DeselectAll()
	{
		selectedEnityIDs.clear();
	}
}