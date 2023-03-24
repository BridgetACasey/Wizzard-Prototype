//@BridgetACasey

#include "wzpch.h"

#include "EntitySelection.h"

namespace Wizzard
{
	void EntitySelection::SelectEntity(Entity entity)
	{
		if (IsSelected(entity))
			return;

		selectedEntities.push_back(entity);
	}

	bool EntitySelection::IsSelected(Entity entity)
	{
		return std::find(selectedEntities.begin(), selectedEntities.end(), entity) != selectedEntities.end();
	}

	void EntitySelection::DeselectEntity(Entity entity)
	{
		auto itr = std::find(selectedEntities.begin(), selectedEntities.end(), entity);

		if (itr == selectedEntities.end())
			return;

		selectedEntities.erase(itr);
	}

	void EntitySelection::DeselectAll()
	{
		if(!selectedEntities.empty())
		selectedEntities.clear();
	}
}