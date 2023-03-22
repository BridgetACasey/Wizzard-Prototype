//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/base/Maths.h"
#include "wizzard/input/KeyCode.h"
#include "wizzard/input/MouseCode.h"
#include "wizzard/input/ControllerCode.h"

namespace Wizzard
{
	constexpr int TOTAL_KEY_CODES = Key::TotalCodeCount;
	constexpr int TOTAL_MOUSE_CODES = Mouse::TotalCodeCount;
	constexpr int TOTAL_CONTROLLER_CODES = Controller::TotalCodeCount;

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

		static bool SetMouseButtonState(MouseCode mouseCode, InputState state);
		static InputState GetMouseButtonState(MouseCode mouseCode);

		//static bool IsControllerButtonPressed(ControllerCode controllerCode);
		//static bool IsControllerButtonReleased(ControllerCode controllerCode);
		//static bool IsControllerButtonUp(ControllerCode controllerCode);
		//static bool IsControllerButtonDown(ControllerCode controllerCode);
		//
		//static bool SetControllerButtonUp(ControllerCode controllerCode);
		//static bool SetControllerButtonDown(ControllerCode controllerCode);

		static bool SetMousePosition(float x, float y);
		static bool SetMousePositionX(float x);
		static bool SetMousePositionY(float y);

		static Maths::WizVector2f GetMousePosition();

		inline static bool queryInput = false;

		/*
		 * TODO: Find a way to make printing code values to the log less verbose than calling Get...CodeAsString from client side every time.
		 */

		static std::string GetKeyCodeAsString(KeyCode keyCode)
		{
			switch(keyCode)
			{
				case Key::Space:		return "Space";
				case Key::Apostrophe:	return "Apostrophe";
				case Key::Comma:		return "Comma";
				case Key::Minus:		return "Minus";
				case Key::Period:		return "Period";
				case Key::Slash:		return "Slash";

				case Key::D0:			return "D0";
				case Key::D1:			return "D1";
				case Key::D2:			return "D2";
				case Key::D3:			return "D3";
				case Key::D4:			return "D4";
				case Key::D5:			return "D5";
				case Key::D6:			return "D6";
				case Key::D7:			return "D7";
				case Key::D8:			return "D8";
				case Key::D9:			return "D9";

				case Key::Semicolon:	return "Semicolon";
				case Key::Equal:		return "Equal";

				case Key::A:			return "A";
				case Key::B:			return "B";
				case Key::C:			return "C";
				case Key::D:			return "D";
				case Key::E:			return "E";
				case Key::F:			return "F";
				case Key::G:			return "G";
				case Key::H:			return "H";
				case Key::I:			return "I";
				case Key::J:			return "J";
				case Key::K:			return "K";
				case Key::L:			return "L";
				case Key::M:			return "M";
				case Key::N:			return "N";
				case Key::O:			return "O";
				case Key::P:			return "P";
				case Key::Q:			return "Q";
				case Key::R:			return "R";
				case Key::S:			return "S";
				case Key::T:			return "T";
				case Key::U:			return "U";
				case Key::V:			return "V";
				case Key::W:			return "W";
				case Key::X:			return "X";
				case Key::Y:			return "Y";
				case Key::Z:			return "Z";

				case Key::LeftBracket:	return "LeftBracket";
				case Key::Backslash:	return "Backslash";
				case Key::RightBracket:	return "RightBracket";
				case Key::GraveAccent:	return "GraveAccent";

				case Key::World1:		return "World1";
				case Key::World2:		return "World2";

				case Key::Escape:		return "Escape";
				case Key::Enter:		return "Enter";
				case Key::Tab:			return "Tab";
				case Key::Backspace:	return "Backspace";
				case Key::Insert:		return "Insert";
				case Key::Delete:		return "Delete";
				case Key::Right:		return "Right";
				case Key::Left:			return "Left";
				case Key::Down:			return "Down";
				case Key::Up:			return "Up";
				case Key::PageUp:		return "PageUp";
				case Key::PageDown:		return "PageDown";
				case Key::Home:			return "Home";
				case Key::End:			return "End";
				case Key::CapsLock:		return "CapsLock";
				case Key::ScrollLock:	return "ScrollLock";
				case Key::NumLock:		return "NumLock";
				case Key::PrintScreen:	return "PrintScreen";
				case Key::Pause:		return "Pause";
				case Key::F1:			return "F1";
				case Key::F2:			return "F2";
				case Key::F3:			return "F3";
				case Key::F4:			return "F4";
				case Key::F5:			return "F5";
				case Key::F6:			return "F6";
				case Key::F7:			return "F7";
				case Key::F8:			return "F8";
				case Key::F9:			return "F9";
				case Key::F10:			return "F10";
				case Key::F11:			return "F11";
				case Key::F12:			return "F12";
				case Key::F13:			return "F13";
				case Key::F14:			return "F14";
				case Key::F15:			return "F15";
				case Key::F16:			return "F16";
				case Key::F17:			return "F17";
				case Key::F18:			return "F18";
				case Key::F19:			return "F19";
				case Key::F20:			return "F20";
				case Key::F21:			return "F21";
				case Key::F22:			return "F22";
				case Key::F23:			return "F23";
				case Key::F24:			return "F24";
				case Key::F25:			return "F25";

				case Key::KP0:			return "KP0";
				case Key::KP1:			return "KP1";
				case Key::KP2:			return "KP2";
				case Key::KP3:			return "KP3";
				case Key::KP4:			return "KP4";
				case Key::KP5:			return "KP5";
				case Key::KP6:			return "KP6";
				case Key::KP7:			return "KP7";
				case Key::KP8:			return "KP8";
				case Key::KP9:			return "KP9";
				case Key::KPDecimal:	return "KPDecimal";
				case Key::KPDivide:		return "KPDivide";
				case Key::KPMultiply:	return "KPMultiply";
				case Key::KPSubtract:	return "KPSubtract";
				case Key::KPAdd:		return "KPAdd";
				case Key::KPEnter:		return "KPEnter";
				case Key::KPEqual:		return "KPEqual";

				case Key::LeftShift:	return "LeftShift";
				case Key::LeftControl:	return "LeftControl";
				case Key::LeftAlt:		return "LeftAlt";
				case Key::LeftSuper:	return "LeftSuper";
				case Key::RightShift:	return "RightShift";
				case Key::RightControl:	return "RightControl";
				case Key::RightAlt:		return "RightAlt";
				case Key::RightSuper:	return "RightSuper";
				case Key::Menu:			return "Menu";

				default:				return "None";
			}
		}

		static std::string GetMouseCodeAsString(MouseCode mouseCode)
		{
			switch(mouseCode)
			{
				case Mouse::LeftButton:		return "LeftButton";
				case Mouse::RightButton:	return "RightButton";
				case Mouse::MiddleButton	:return "MiddleButton";
				case Mouse::LastButton:		return "LastButton";

				case Mouse::Button3:		return "Button3";
				case Mouse::Button4:		return "Button4";
				case Mouse::Button5:		return "Button5";
				case Mouse::Button6:		return "Button6";

				default:					return "None";
			}
		}

		//TODO
		static std::string GetControllerCodeAsString(ControllerCode controllerCode)
		{
			switch(controllerCode)
			{
				default: return "None";
			}
		}

		static std::string GetInputStateAsString(InputState inputState)
		{
			switch(inputState)
			{
				case PRESSED:	return "PRESSED";
				case RELEASED:	return "RELEASED";
				case DOWN:		return "DOWN";
				case UP:		return "UP";

				default:		return "NONE";
			}
		}

	private:
		inline static bool initInput = false;

		static bool IsQueryingInput()
		{
			if (!initInput)
			{
				WIZ_ERROR("Wizzard::Input class not initialised! Call Input::Init before attempting to query input.");
				return false;
			}

			if (!queryInput)
			{
				WIZ_WARN("Trying to query input when 'queryInput' flag is set to FALSE! This flag is found in the Wizzard::Input class.");
				return false;
			}

			return true;
		}
	};
}