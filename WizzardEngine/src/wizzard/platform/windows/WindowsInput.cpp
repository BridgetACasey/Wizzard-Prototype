//@BridgetACasey

#include "wzpch.h"

#include "input/Input.h"
#include "wizzard/common/Application.h"
#include "GLFW/glfw3.h"

namespace Wizzard
{
	//Storing the states for each unique key and mouse button to check if it has only been pressed once
	static std::unordered_map<KeyCode, InputState> keyStates;
	static std::unordered_map<MouseCode, InputState> mouseStates;

	//TODO: Check the key or mouse button is a valid code before trying to fetch and modify its value, i.e., it's not None or TotalCodeCount.

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		bool pressed = keyStates.at(keyCode);

		//If the key was already detected as being pressed and it's still down, don't trigger again until it has been released.
		if(state == GLFW_PRESS)
		{
			if (pressed == PRESSED)
				return false;

			keyStates.at(keyCode) = PRESSED;
			pressed = true;
		}
		else
		{
			keyStates.at(keyCode) = RELEASED;
			pressed = false;
		}

		return pressed;
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		bool released = false;

		if (state == GLFW_PRESS || state == GLFW_REPEAT)
			keyStates.at(keyCode) = DOWN;

		else if (state == GLFW_RELEASE && keyStates.at(keyCode) == DOWN)
		{
			keyStates.at(keyCode) = RELEASED;
			released = true;
		}

		return released;
	}

	bool Input::IsKeyUp(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		bool up = false;

		if(state == GLFW_RELEASE)
		{
			keyStates.at(keyCode) = UP;
			up = true;
		}

		return up;
	}

	bool Input::IsKeyDown(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		bool down = false;

		if(state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			keyStates.at(keyCode) = DOWN;
			down = true;
		}

		return down;
	}

	bool Input::SetKeyUp(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		keyStates.at(keyCode) = UP;

		return true;
	}

	bool Input::SetKeyDown(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return false;

		keyStates.at(keyCode) = DOWN;

		return true;
	}

	bool Input::SetKeyState(KeyCode keyCode, InputState state)
	{
		if (!IsQueryingInput())
			return false;

		keyStates.at(keyCode) = state;

		return true;
	}

	InputState Input::GetKeyState(KeyCode keyCode)
	{
		if (!IsQueryingInput())
			return NONE;

		return keyStates.at(keyCode);
	}

	bool Input::IsMouseButtonPressed(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);
		bool pressed = mouseStates.at(mouseCode);

		//If the mouse button was already detected as being pressed and it's still down, don't trigger again until it has been released.
		if (state == GLFW_PRESS)
		{
			if (pressed == PRESSED)
				return false;

			mouseStates.at(mouseCode) = PRESSED;
			pressed = true;
		}
		else
		{
			mouseStates.at(mouseCode) = RELEASED;
			pressed = false;
		}

		return pressed;
	}

	bool Input::IsMouseButtonReleased(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		bool released = false;

		if (state == GLFW_PRESS || state == GLFW_REPEAT)
			mouseStates.at(mouseCode) = DOWN;

		else if (state == GLFW_RELEASE && mouseStates.at(mouseCode) == DOWN)
		{
			mouseStates.at(mouseCode) = RELEASED;
			released = true;
		}

		return released;
	}

	bool Input::IsMouseButtonUp(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		bool up = false;

		if (state == GLFW_RELEASE)
		{
			mouseStates.at(mouseCode) = UP;
			up = true;
		}

		return up;
	}

	bool Input::IsMouseButtonDown(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		bool down = false;

		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			mouseStates.at(mouseCode) = DOWN;
			down = true;
		}

		return down;
	}

	bool Input::SetMouseButtonUp(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		mouseStates.at(mouseCode) = UP;

		return true;
	}

	bool Input::SetMouseButtonDown(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return false;

		mouseStates.at(mouseCode) = DOWN;

		return true;
	}

	bool Input::SetMousePosition(float x, float y)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		glfwSetCursorPos(window, (double)x, (double)y);

		return true;
	}

	bool Input::SetMousePositionX(float x)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double currentX, currentY;
		glfwGetCursorPos(window, &currentX, &currentY);

		glfwSetCursorPos(window, (double)x, currentY);

		return true;
	}

	bool Input::SetMousePositionY(float y)
	{
		if (!IsQueryingInput())
			return false;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double currentX, currentY;
		glfwGetCursorPos(window, &currentX, &currentY);

		glfwSetCursorPos(window, currentX, (double)y);

		return true;
	}

	bool Input::SetMouseButtonState(MouseCode mouseCode, InputState state)
	{
		if (!IsQueryingInput())
			return false;

		mouseStates.at(mouseCode) = state;

		return true;
	}

	InputState Input::GetMouseButtonState(MouseCode mouseCode)
	{
		if (!IsQueryingInput())
			return NONE;

		return mouseStates.at(mouseCode);
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double posX, posY;

		glfwGetCursorPos(window, &posX, &posY);

		return { (float)posX, (float)posY };
	}

	float Input::GetMousePositionX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMousePositionY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}

	void Input::Init()
	{
		//Having to insert each key and button value manually as they are specified in their enums, so can't insert via a for loop

		//Inserting key states
		keyStates.insert(std::pair(Key::Space, UP));
		keyStates.insert(std::pair(Key::Apostrophe, UP));
		keyStates.insert(std::pair(Key::Comma, UP));
		keyStates.insert(std::pair(Key::Minus, UP));
		keyStates.insert(std::pair(Key::Period, UP));
		keyStates.insert(std::pair(Key::Slash, UP));

		keyStates.insert(std::pair(Key::D0, UP));
		keyStates.insert(std::pair(Key::D1, UP));
		keyStates.insert(std::pair(Key::D2, UP));
		keyStates.insert(std::pair(Key::D3, UP));
		keyStates.insert(std::pair(Key::D4, UP));
		keyStates.insert(std::pair(Key::D5, UP));
		keyStates.insert(std::pair(Key::D6, UP));
		keyStates.insert(std::pair(Key::D7, UP));
		keyStates.insert(std::pair(Key::D8, UP));
		keyStates.insert(std::pair(Key::D9, UP));

		keyStates.insert(std::pair(Key::Semicolon, UP));
		keyStates.insert(std::pair(Key::Equal, UP));

		keyStates.insert(std::pair(Key::A, UP));
		keyStates.insert(std::pair(Key::B, UP));
		keyStates.insert(std::pair(Key::C, UP));
		keyStates.insert(std::pair(Key::D, UP));
		keyStates.insert(std::pair(Key::E, UP));
		keyStates.insert(std::pair(Key::F, UP));
		keyStates.insert(std::pair(Key::G, UP));
		keyStates.insert(std::pair(Key::H, UP));
		keyStates.insert(std::pair(Key::I, UP));
		keyStates.insert(std::pair(Key::J, UP));
		keyStates.insert(std::pair(Key::K, UP));
		keyStates.insert(std::pair(Key::L, UP));
		keyStates.insert(std::pair(Key::M, UP));
		keyStates.insert(std::pair(Key::N, UP));
		keyStates.insert(std::pair(Key::O, UP));
		keyStates.insert(std::pair(Key::P, UP));
		keyStates.insert(std::pair(Key::Q, UP));
		keyStates.insert(std::pair(Key::R, UP));
		keyStates.insert(std::pair(Key::S, UP));
		keyStates.insert(std::pair(Key::T, UP));
		keyStates.insert(std::pair(Key::U, UP));
		keyStates.insert(std::pair(Key::V, UP));
		keyStates.insert(std::pair(Key::W, UP));
		keyStates.insert(std::pair(Key::X, UP));
		keyStates.insert(std::pair(Key::Y, UP));
		keyStates.insert(std::pair(Key::Z, UP));

		keyStates.insert(std::pair(Key::LeftBracket, UP));
		keyStates.insert(std::pair(Key::Backslash, UP));
		keyStates.insert(std::pair(Key::RightBracket, UP));
		keyStates.insert(std::pair(Key::GraveAccent, UP));

		keyStates.insert(std::pair(Key::World1, UP));
		keyStates.insert(std::pair(Key::World2, UP));

		keyStates.insert(std::pair(Key::Escape, UP));
		keyStates.insert(std::pair(Key::Enter, UP));
		keyStates.insert(std::pair(Key::Tab, UP));
		keyStates.insert(std::pair(Key::Backspace, UP));
		keyStates.insert(std::pair(Key::Insert, UP));
		keyStates.insert(std::pair(Key::Delete, UP));
		keyStates.insert(std::pair(Key::Right, UP));
		keyStates.insert(std::pair(Key::Left, UP));
		keyStates.insert(std::pair(Key::Down, UP));
		keyStates.insert(std::pair(Key::Up, UP));
		keyStates.insert(std::pair(Key::PageUp, UP));
		keyStates.insert(std::pair(Key::PageDown, UP));
		keyStates.insert(std::pair(Key::Home, UP));
		keyStates.insert(std::pair(Key::End, UP));
		keyStates.insert(std::pair(Key::CapsLock, UP));
		keyStates.insert(std::pair(Key::ScrollLock, UP));
		keyStates.insert(std::pair(Key::NumLock, UP));
		keyStates.insert(std::pair(Key::PrintScreen, UP));
		keyStates.insert(std::pair(Key::Pause, UP));

		keyStates.insert(std::pair(Key::F1, UP));
		keyStates.insert(std::pair(Key::F2, UP));
		keyStates.insert(std::pair(Key::F3, UP));
		keyStates.insert(std::pair(Key::F4, UP));
		keyStates.insert(std::pair(Key::F5, UP));
		keyStates.insert(std::pair(Key::F6, UP));
		keyStates.insert(std::pair(Key::F7, UP));
		keyStates.insert(std::pair(Key::F8, UP));
		keyStates.insert(std::pair(Key::F9, UP));
		keyStates.insert(std::pair(Key::F10, UP));
		keyStates.insert(std::pair(Key::F11, UP));
		keyStates.insert(std::pair(Key::F12, UP));
		keyStates.insert(std::pair(Key::F13, UP));
		keyStates.insert(std::pair(Key::F14, UP));
		keyStates.insert(std::pair(Key::F15, UP));
		keyStates.insert(std::pair(Key::F16, UP));
		keyStates.insert(std::pair(Key::F17, UP));
		keyStates.insert(std::pair(Key::F18, UP));
		keyStates.insert(std::pair(Key::F19, UP));
		keyStates.insert(std::pair(Key::F20, UP));
		keyStates.insert(std::pair(Key::F21, UP));
		keyStates.insert(std::pair(Key::F22, UP));
		keyStates.insert(std::pair(Key::F23, UP));
		keyStates.insert(std::pair(Key::F24, UP));
		keyStates.insert(std::pair(Key::F25, UP));

		keyStates.insert(std::pair(Key::KP0, UP));
		keyStates.insert(std::pair(Key::KP1, UP));
		keyStates.insert(std::pair(Key::KP2, UP));
		keyStates.insert(std::pair(Key::KP3, UP));
		keyStates.insert(std::pair(Key::KP4, UP));
		keyStates.insert(std::pair(Key::KP5, UP));
		keyStates.insert(std::pair(Key::KP6, UP));
		keyStates.insert(std::pair(Key::KP7, UP));
		keyStates.insert(std::pair(Key::KP8, UP));
		keyStates.insert(std::pair(Key::KP9, UP));
		keyStates.insert(std::pair(Key::KPDecimal, UP));
		keyStates.insert(std::pair(Key::KPDivide, UP));
		keyStates.insert(std::pair(Key::KPMultiply, UP));
		keyStates.insert(std::pair(Key::KPSubtract, UP));
		keyStates.insert(std::pair(Key::KPAdd, UP));
		keyStates.insert(std::pair(Key::KPEnter, UP));
		keyStates.insert(std::pair(Key::KPEqual, UP));

		keyStates.insert(std::pair(Key::LeftShift, UP));
		keyStates.insert(std::pair(Key::LeftControl, UP));
		keyStates.insert(std::pair(Key::LeftAlt, UP));
		keyStates.insert(std::pair(Key::LeftSuper, UP));
		keyStates.insert(std::pair(Key::RightShift, UP));
		keyStates.insert(std::pair(Key::RightControl, UP));
		keyStates.insert(std::pair(Key::RightAlt, UP));
		keyStates.insert(std::pair(Key::RightSuper, UP));
		keyStates.insert(std::pair(Key::Menu, UP));

		//Inserting mouse states
		mouseStates.insert(std::pair(Mouse::Button0, UP));
		mouseStates.insert(std::pair(Mouse::Button1, UP));
		mouseStates.insert(std::pair(Mouse::Button2, UP));
		mouseStates.insert(std::pair(Mouse::Button3, UP));
		mouseStates.insert(std::pair(Mouse::Button4, UP));
		mouseStates.insert(std::pair(Mouse::Button5, UP));
		mouseStates.insert(std::pair(Mouse::Button6, UP));
		mouseStates.insert(std::pair(Mouse::Button7, UP));

		mouseStates.insert(std::pair(Mouse::LeftButton, UP));
		mouseStates.insert(std::pair(Mouse::RightButton, UP));
		mouseStates.insert(std::pair(Mouse::MiddleButton, UP));
		mouseStates.insert(std::pair(Mouse::LastButton, UP));

		queryInput = true;
		initInput = true;
	}

	void Input::Shutdown()
	{
		keyStates.clear();
		mouseStates.clear();

		queryInput = false;
		initInput = false;
	}
}