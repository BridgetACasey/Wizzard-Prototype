//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/event/EditorEvent.h"
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel() = default;

		void OnImGuiRender() override;

		Entity GetSelectedEntity() const { return selectionContext; }
		void SetSelectedEntity(Entity entity) { selectionContext = entity; }

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawComponents(Entity entity);

		Entity selectionContext = {};
	};
}