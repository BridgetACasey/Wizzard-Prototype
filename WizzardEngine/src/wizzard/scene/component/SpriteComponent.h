//@BridgetACasey

#pragma once

#include <glm/glm.hpp>

namespace Wizzard
{
	struct SpriteComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color) : color(color) {}
	};
}