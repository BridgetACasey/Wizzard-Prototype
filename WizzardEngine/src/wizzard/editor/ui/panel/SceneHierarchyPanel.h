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

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		void DrawEntityNode(Entity entity);

		bool OnKeyPressed(KeyPressedEvent& keyEvent);

		bool openEntityCreationWindow = false;
		bool openEntityGroupWindow = false;
	};
}