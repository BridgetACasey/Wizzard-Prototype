//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class ViewportPanel : public EditorPanel
	{
	public:
		ViewportPanel() = default;
		~ViewportPanel();

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

		//Temp??
		void SetGizmoType(int type) { gizmoType = type; }
		glm::vec2& GetViewportSize() { return viewportSize; }
		glm::vec2* GetViewportBounds() { return viewportBounds; }

	private:
		bool OnKeyPressed(KeyPressedEvent& keyEvent);

		void OnViewportToolbarRender();

		int gizmoType = -1;

		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];
	};
}