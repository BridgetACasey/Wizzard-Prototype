//@BridgetACasey

#pragma once

#include "wizzard/rendering/Camera.h"
#include "wizzard/base/TimeStep.h"
#include "wizzard/event/Event.h"
#include "wizzard/event/MouseEvent.h"
#include "wizzard/event/KeyEvent.h"
#include "wizzard/event/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Wizzard
{
	class ViewportSelectionChangedEvent;

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		void FocusOnPoint(const glm::vec3& point);

		inline float GetDistance() const { return distance; }
		inline void SetDistance(float dist) { distance = dist; }

		inline void SetViewportSize(float width, float height) { viewportWidth = width; viewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return viewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }

		void SetEnableUserControl(bool status) { enableUserControl = status; }
		bool GetEnableUserControl() const { return enableUserControl; }

		void SetZoomOnlyControl(bool status) { zoomOnlyControl = status; }
		bool GetZoomOnlyControl() const { return zoomOnlyControl; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& keyEvent);
		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);
		bool OnWindowClose(WindowCloseEvent& windowEvent);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		void SmoothAutoPan(const glm::vec3& target);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float fov = 45.0f, aspectRatio = 1.778f, nearClip = 0.1f, farClip = 1000.0f;

		glm::mat4 viewMatrix;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 initialMousePosition = { 0.0f, 0.0f };

		float distance = 10.0f;
		float pitch = 0.0f, yaw = 0.0f;

		float viewportWidth = 0.0f, viewportHeight = 0.0f;

		bool enableUserControl = true;
		bool zoomOnlyControl = false;

		float smoothPanSpeed = 1.5f;
	};
}
