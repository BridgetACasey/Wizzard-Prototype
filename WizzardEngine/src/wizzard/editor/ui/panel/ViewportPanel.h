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

		void SetMoveUnitCount(int unitCount) { moveUnitCount = unitCount; }
		void SetEntityBaseTranslation(glm::vec3& translation) { entityBaseTranslation = translation; }
		void SetEntityBaseRotation(glm::vec3& rotation) { entityBaseRotation = rotation; }
		void SetEntityBaseScale(glm::vec3& scale) { entityBaseScale = scale; }

	private:
		bool OnKeyPressed(KeyPressedEvent& keyEvent);

		int gizmoType = -1;

		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		bool canTriggerTransform = true;
		bool isTransformActive = false;
		float snapValue = 0.5f;// Snap to 0.5m for translation/scale

		int moveUnitCount = 0;	//How many units we have moved our selected entities, used for updating position info to screen reader

		glm::vec3 entityBaseTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 entityBaseRotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 entityBaseScale = { 0.0f, 0.0f, 0.0f };
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