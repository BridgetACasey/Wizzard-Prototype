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

	private:
		void DrawComponents(Entity entity);
		void DrawEntityNode(Entity entity);

		Entity selectionContext = {};
	};
}