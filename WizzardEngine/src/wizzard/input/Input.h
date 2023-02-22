//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/input/KeyCode.h"
#include "wizzard/input/MouseCode.h"

namespace Wizzard
{
	constexpr int TOTAL_KEY_CODES = Key::TotalCodeCount;
	constexpr int TOTAL_MOUSE_CODES = Mouse::TotalCodeCount;

	enum InputState
	{
		NONE = -1,
		PRESSED,
		RELEASED, //RELEASE is distinguished from UP as it first checks if the key/button has been recently pressed
		UP,
		DOWN
	};

	/*
	 * NOTE: Setting the key or mouse button state manually is possible, but not recommended.
	 *		 Prefer querying the input state first, so as not to conflict with event callbacks.
	 */

	class Input
	{
	public:
		static void Init();
		static void Shutdown();

		static bool IsKeyPressed(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);
		static bool IsKeyUp(KeyCode keyCode);
		static bool IsKeyDown(KeyCode keyCode);

		static bool SetKeyUp(KeyCode keyCode);
		static bool SetKeyDown(KeyCode keyCode);

		static InputState GetKeyState(KeyCode keyCode);

		static bool IsMouseButtonPressed(MouseCode mouseCode);
		static bool IsMouseButtonReleased(MouseCode mouseCode);
		static bool IsMouseButtonUp(MouseCode mouseCode);
		static bool IsMouseButtonDown(MouseCode mouseCode);

		static bool SetMouseButtonUp(MouseCode mouseCode);
		static bool SetMouseButtonDown(MouseCode mouseCode);

		static bool SetMousePosition(float x, float y);
		static bool SetMousePositionX(float x);
		static bool SetMousePositionY(float y);

		static InputState GetMouseButtonState(MouseCode mouseCode);

		static std::pair<float, float> GetMousePosition();

		static float GetMousePositionX();
		static float GetMousePositionY();
	};
}