//@BridgetACasey

#pragma once

#include <glm/glm.hpp>

#include "wizzard/scene/SceneCamera.h"

namespace Wizzard
{
	struct CameraComponent
	{
		SceneCamera Camera;

		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}