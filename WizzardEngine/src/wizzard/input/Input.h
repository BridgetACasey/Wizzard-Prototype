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
		PRESSED,	//PRESSED is distinguished from DOWN as it registers only the first frame an input is detected
		RELEASED, //RELEASE is distinguished from UP as it first checks if the key/button has been recently pressed or held
		UP,
		DOWN
	};

	/*
	 * NOTE: Setting the key or mouse button state manually is possible, but not recommended.
	 *		 Prefer querying the input state first, so as not to conflict with event callbacks.
	 */

	//TODO: Add functions to convert input values and state values to a written form in the logger, i.e., 'Left Mouse Button' instead of '0'

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

		static bool SetKeyState(KeyCode keyCode, InputState state);
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

		static bool SetMouseButtonState(MouseCode mouseCode, InputState state);
		static InputState GetMouseButtonState(MouseCode mouseCode);

		static std::pair<float, float> GetMousePosition();

		static float GetMousePositionX();
		static float GetMousePositionY();

		inline static bool queryInput = false;

	private:
		inline static bool initInput = false;

		static bool IsQueryingInput()
		{
			if(!initInput)
			{
				WIZ_ERROR("Wizzard::Input class not initialised! Call Input::Init before attempting to query input.");
				return false;
			}

			if(!queryInput)
			{
				WIZ_WARN("Trying to query input when 'queryInput' flag is set to FALSE! This flag is found in the Wizzard::Input class.");
				return false;
			}

			return true;
		}

		//Not yet implemented
		static void GetKeyCodeAsString(KeyCode keyCode)
		{
			/*
			 * ...
			 */
		}

		//Not yet implemented
		static void GetMouseCodeAsString(MouseCode mouseCode)
		{
			/*
			 * ...
			 */
		}
	};
}