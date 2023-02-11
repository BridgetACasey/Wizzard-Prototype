//@BridgetACasey

#include "wzpch.h"
#include "OrthographicCameraController.h"

#include "Input.h"
#include "event/EventHandler.h"

namespace Wizzard
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio* zoomLevel, -zoomLevel, zoomLevel), rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timeStep)
	{
		if (Input::IsKeyPressed(Key::A))
			cameraPosition.x -= cameraTranslationSpeed * timeStep;
		else if (Input::IsKeyPressed(Key::D))
			cameraPosition.x += cameraTranslationSpeed * timeStep;

		if (Input::IsKeyPressed(Key::W))
			cameraPosition.y += cameraTranslationSpeed * timeStep;
		else if (Input::IsKeyPressed(Key::S))
			cameraPosition.y -= cameraTranslationSpeed * timeStep;

		if (rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				cameraRotation += cameraRotationSpeed * timeStep;
			if (Input::IsKeyPressed(Key::E))
				cameraRotation -= cameraRotationSpeed * timeStep;

			camera.SetRotation(cameraRotation);
		}

		camera.SetPosition(cameraPosition);

		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventHandler dispatcher(event);
		dispatcher.HandleEvent<MouseScrolledEvent>(WIZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.HandleEvent<WindowResizeEvent>(WIZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		zoomLevel -= event.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		aspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		camera.SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}