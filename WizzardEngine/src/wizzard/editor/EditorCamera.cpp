//@BridgetACasey

#include "wzpch.h"

#include "EditorCamera.h"

#include "wizzard/input/Input.h"
#include "wizzard/input/KeyCode.h"
#include "wizzard/input/MouseCode.h"
#include "wizzard/event/EventHandler.h"
#include "wizzard/event/EditorEvent.h"

#include "audio/Audio.h"
#include "audio/AudioSource.h"
#include "base/ResourcePathFinder.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "wizzard/scene/component/TransformComponent.h"

namespace Wizzard
{
	//TODO: Move these somewhere more sensible
	static AudioSource zoomInSFX;
	static AudioSource zoomOutSFX;

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();

		if (Audio::IsActive())
		{
			zoomInSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "178187__snapper4298__lens-zooming-in.mp3"), false);
			zoomOutSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "178187__snapper4298__lens-zooming-out.mp3"), false);
		}
		else
			WIZ_ERROR("Could not initialise SFX for EditorCamera!");
	}

	void EditorCamera::UpdateProjection()
	{
		aspectRatio = viewportWidth / viewportHeight;
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	void EditorCamera::UpdateView()
	{
		// yaw = pitch = 0.0f; // Lock the camera's rotation
		position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float dist = distance * 0.2f;
		dist = std::max(dist, 0.0f);
		float speed = dist * dist;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		smoothPanSpeed = 1.5f * ts;

		if (enableUserControl)
		{
			if (Input::IsKeyDown(Key::LeftAlt))
			{
				const glm::vec2& mouse{ Input::GetMousePosition().x, Input::GetMousePosition().y };
				glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
				initialMousePosition = mouse;

				if (Input::IsMouseButtonDown(Mouse::MiddleButton))
					MousePan(delta);
				else if (Input::IsMouseButtonDown(Mouse::LeftButton))
					MouseRotate(delta);
				else if (Input::IsMouseButtonDown(Mouse::RightButton))
					MouseZoom(delta.y);
			}
			else
			{
				glm::vec2 delta = { 0.0f, 0.0f };
				constexpr float panSpeed = 1.5f;

				if (Input::IsKeyDown(Key::Left))
					delta.x += panSpeed * ts;
				if (Input::IsKeyDown(Key::Right))
					delta.x -= panSpeed * ts;
				if (Input::IsKeyDown(Key::Up))
					delta.y += panSpeed * ts;
				if (Input::IsKeyDown(Key::Down))
					delta.y -= panSpeed * ts;

				if (delta.x || delta.y)
					MousePan(delta);
			}
		}

		if(zoomOnlyControl || enableUserControl)
		{
			if (Input::IsKeyDown(Key::LeftBracket))
			{
				float delta = -1.5f * ts;	//TODO: Set default values for keyboard zoom/pan/rotate speeds
				MouseZoom(delta);
			}
			else if (Input::IsKeyDown(Key::RightBracket))
			{
				float delta = 1.5f * ts;
				MouseZoom(delta);
			}
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventHandler handler(e);
		handler.HandleEvent<MouseScrolledEvent>(WIZ_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
		handler.HandleEvent<KeyPressedEvent>(WIZ_BIND_EVENT_FN(EditorCamera::OnKeyPressed));
		handler.HandleEvent<WindowCloseEvent>(WIZ_BIND_EVENT_FN(EditorCamera::OnWindowClose));
		handler.HandleEvent<ViewportSelectionChangedEvent>(WIZ_BIND_EVENT_FN(EditorCamera::OnViewportSelectionChanged));
	}

	void EditorCamera::FocusOnPoint(const glm::vec3& point)
	{
		focalPoint = point;
		UpdateView();
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;

		MouseZoom(delta);
		UpdateView();

		Audio::Play(zoomInSFX);

		return false;
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& keyEvent)
	{
		if (Input::IsQueryingInput())
		{
			if (zoomOnlyControl || enableUserControl)
			{
				if (keyEvent.GetKeyCode() == Key::LeftBracket)
				{
					Audio::Play(zoomInSFX);
				}
				else if (keyEvent.GetKeyCode() == Key::RightBracket)
				{
					Audio::Play(zoomOutSFX);
				}
			}
		}

		return false;
	}

	bool EditorCamera::OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent)
	{
		if(sceneEvent.GetSelectionContext())
		SmoothAutoPan(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Translation);

		return false;
	}

	bool EditorCamera::OnWindowClose(WindowCloseEvent& windowEvent)
	{
		zoomInSFX.FreeSource();	//TODO: Refactor this so freeing sfx is easier and more sensible
		zoomOutSFX.FreeSource();

		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		focalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
		focalPoint += GetUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * RotationSpeed();
		pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		distance -= delta * ZoomSpeed();
		if (distance < 1.0f)
		{
			focalPoint += GetForwardDirection();
			distance = 1.0f;
		}
	}

	void EditorCamera::SmoothAutoPan(const glm::vec3& target)
	{
		//TODO: Fix smooth camera panning
		glm::lerp(glm::quat(focalPoint), glm::quat(target), smoothPanSpeed);
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return focalPoint - GetForwardDirection() * distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}
}