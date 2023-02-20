//@BridgetACasey

#pragma once

#include <unordered_map>

#include "wizzard/base/Base.h"
#include "wizzard/input/KeyCode.h"
#include "wizzard/input/MouseCode.h"

namespace Wizzard
{
	class Input
	{
	public:
		static void Init();

		static bool IsKeyPressed(KeyCode keyCode);
		static bool IsKeyDown(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);

		static bool IsMouseButtonPressed(MouseCode mouseCode);
		static bool IsMouseButtonDown(MouseCode mouseCode);
		static bool IsMouseButtonReleased(MouseCode mouseCode);

		static std::pair<float, float> GetMousePosition();

		static float GetMouseX();
		static float GetMouseY();
	};
}