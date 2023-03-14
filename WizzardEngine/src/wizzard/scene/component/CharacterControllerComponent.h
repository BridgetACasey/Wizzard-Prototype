//@BridgetACasey

#pragma once

namespace Wizzard
{
	struct CharacterControllerComponent
	{
		CharacterControllerComponent() = default;
		CharacterControllerComponent(const CharacterControllerComponent&) = default;

		bool disableGravity = false;
	};
}