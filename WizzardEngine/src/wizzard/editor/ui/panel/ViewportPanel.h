//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class AudioSource;

	class ViewportPanel : public EditorPanel
	{
	public:
		ViewportPanel() = default;
		~ViewportPanel();

		void OnUpdate(TimeStep timeStep) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

		//Temp??
		void SetGizmoType(int type) { gizmoType = type; }
		glm::vec2& GetViewportSize() { return viewportSize; }
		glm::vec2* GetViewportBounds() { return viewportBounds; }
		void SetEntityOrigin(glm::vec3& pos) { entityOrigin = pos; }

	private:
		bool OnKeyPressed(KeyPressedEvent& keyEvent);

		int gizmoType = -1;

		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		bool canTriggerTransform = true;
		bool isTransformActive = false;
		float snapValue = 0.5f;// Snap to 0.5m for translation/scale

		glm::vec3 entityOrigin = {0.0f, 0.0f, 0.0f};
	};

	class ViewportToolbarPanel : public EditorPanel
	{
	public:
		ViewportToolbarPanel() = default;
		~ViewportToolbarPanel();

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;
	};
}