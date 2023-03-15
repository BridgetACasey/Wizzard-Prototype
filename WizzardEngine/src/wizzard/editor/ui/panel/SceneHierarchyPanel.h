//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		SceneHierarchyPanel() = default;

		void OnImGuiRender() override;

		Entity GetSelectedEntity() const { return selectionContext; }
		void SetSelectedEntity(Entity entity);
		void SetSelectedEntityToDefault();

	private:
		void DrawComponents(Entity entity);
		void DrawEntityNode(Entity entity);

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		Entity selectionContext = {};
		Entity defaultEntity = {};
	};
}