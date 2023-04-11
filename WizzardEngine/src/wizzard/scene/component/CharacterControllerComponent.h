//@BridgetACasey

#pragma once

namespace Wizzard
{
	struct CharacterControllerComponent
	{
		CharacterControllerComponent() = default;
		CharacterControllerComponent(const CharacterControllerComponent&) = default;

		//Available in editor
		bool disableGravity = false;

		//Hidden from editor
		bool canJump = true;
		glm::vec3 defaultPosition = {0.0f, 1.0f, 0.0f};
	};
}