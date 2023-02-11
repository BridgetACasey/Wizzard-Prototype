//@BridgetACasey

#pragma once

#include "wizzard/rendering/OrthographicCamera.h"
#include "Timestep.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/MouseEvent.h"

namespace Wizzard
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timeStep);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return camera; }

		float GetZoomLevel() const { return zoomLevel; }
		void SetZoomLevel(float level) { zoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		float aspectRatio;
		float zoomLevel = 1.0f;

		OrthographicCamera camera;

		bool rotation;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		float cameraRotation = 0.0f;
		float cameraTranslationSpeed = 5.0f;
		float cameraRotationSpeed = 180.0f;
	};
}
