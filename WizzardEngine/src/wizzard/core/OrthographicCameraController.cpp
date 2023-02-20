//@BridgetACasey

#include "wzpch.h"
#include "OrthographicCameraController.h"

#include "wizzard/input/Input.h"
#include "event/EventHandler.h"

namespace Wizzard
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio* zoomLevel, -zoomLevel, zoomLevel), rotation(rotation)
	{
		WIZ_PROFILE_FUNCTION();
	}

	void OrthographicCameraController::OnUpdate(TimeStep timeStep)
	{
		WIZ_PROFILE_FUNCTION();

		if (Input::IsKeyDown(Key::A))
			cameraPosition.x -= cameraTranslationSpeed * timeStep;
		else if (Input::IsKeyDown(Key::D))
			cameraPosition.x += cameraTranslationSpeed * timeStep;

		if (Input::IsKeyDown(Key::W))
			cameraPosition.y += cameraTranslationSpeed * timeStep;
		else if (Input::IsKeyDown(Key::S))
			cameraPosition.y -= cameraTranslationSpeed * timeStep;

		if (rotation)
		{
			if (Input::IsKeyDown(Key::Q))
				cameraRotation += cameraRotationSpeed * timeStep;
			if (Input::IsKeyDown(Key::E))
				cameraRotation -= cameraRotationSpeed * timeStep;

			camera.SetRotation(cameraRotation);
		}

		camera.SetPosition(cameraPosition);

		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		WIZ_PROFILE_FUNCTION();

		EventHandler dispatcher(event);
		dispatcher.HandleEvent<MouseScrolledEvent>(WIZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.HandleEvent<WindowResizeEvent>(WIZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		WIZ_PROFILE_FUNCTION();

		zoomLevel -= event.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		WIZ_PROFILE_FUNCTION();

		aspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}