//@BridgetACasey

#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Wizzard
{
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return translate(glm::mat4(1.0f), Translation) * rotation * scale(glm::mat4(1.0f), Scale);
		}
	};
}
