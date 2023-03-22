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

		void OnImGuiRender() override;

	private:
		void OnViewportToolbarRender();

		int gizmoType = -1;

		bool isViewportFocused = false;
		bool isViewportHovered = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];
	};
}