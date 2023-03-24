//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/event/EditorEvent.h"
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		SceneHierarchyPanel() = default;

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		void DrawEntityNode(Entity entity);

		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);

		//Entity selectionContext = {};
		Entity defaultEntity = {};
	};
}