//@BridgetACasey

#include "wzpch.h"

#include "input/Input.h"
#include "wizzard/core/Application.h"
#include "GLFW/glfw3.h"

namespace Wizzard
{
	//Storing the states for each unique key and mouse button to check if it has only been pressed once
	static std::unordered_map<KeyCode, bool> keyStates;
	static std::unordered_map<MouseCode, bool> mouseStates;

	//TODO: Check the key or mouse button is a valid code before trying to fetch and modify its value, i.e., it's not None or TotalCodeCount.

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);
		bool pressed = keyStates.at(keyCode);

		//If the key was already detected as being pressed and it's still down, don't trigger again until it has been released.
		if (pressed && state == GLFW_PRESS)
			return false;

		if(!pressed && state == GLFW_PRESS)
		{
			keyStates.at(keyCode) = true;
			pressed = true;
		}
		else
		{
			keyStates.at(keyCode) = false;
			pressed = false;
		}

		return pressed;
	}

	bool Input::IsKeyUp(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_RELEASE;
	}

	bool Input::IsKeyDown(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	//TODO: Does not check if key has been recently pressed first, fix this.
	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		if (state == GLFW_RELEASE)
			keyStates.at(keyCode) = false;

		const bool pressed = keyStates.at(keyCode);

		return pressed;
	}

	bool Input::SetKeyUp(KeyCode keyCode)
	{
		keyStates.at(keyCode) = false;

		return true;
	}

	bool Input::SetKeyDown(KeyCode keyCode)
	{
		keyStates.at(keyCode) = true;

		return true;
	}

	InputState Input::GetKeyState(KeyCode keyCode)
	{
		return InputState();
	}

	bool Input::IsMouseButtonPressed(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);
		bool pressed = mouseStates.at(mouseCode);

		//If the mouse button was already detected as being pressed and it's still down, don't trigger again until it has been released.
		if (pressed && state == GLFW_PRESS)
			return false;

		if (!pressed && state == GLFW_PRESS)
		{
			mouseStates.at(mouseCode) = true;
			pressed = true;
		}
		else
		{
			mouseStates.at(mouseCode) = false;
			pressed = false;
		}

		return pressed;
	}

	bool Input::IsMouseButtonUp(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, mouseCode);

		return state == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonDown(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	//TODO: Does not check if mouse button has been recently pressed first, fix this.
	bool Input::IsMouseButtonReleased(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_RELEASE;
	}

	bool Input::SetMouseButtonUp(MouseCode mouseCode)
	{
		mouseStates.at(mouseCode) = false;

		return true;
	}

	bool Input::SetMouseButtonDown(MouseCode mouseCode)
	{
		mouseStates.at(mouseCode) = true;

		return true;
	}

	bool Input::SetMousePosition(float x, float y)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		glfwSetCursorPos(window, (double)x, (double)y);

		return true;
	}

	bool Input::SetMousePositionX(float x)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double currentX, currentY;
		glfwGetCursorPos(window, &currentX, &currentY);

		glfwSetCursorPos(window, (double)x, currentY);

		return true;
	}

	bool Input::SetMousePositionY(float y)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double currentX, currentY;
		glfwGetCursorPos(window, &currentX, &currentY);

		glfwSetCursorPos(window, currentX, (double)y);

		return true;
	}

	InputState Input::GetMouseButtonState(MouseCode mouseCode)
	{
		return InputState();
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
		keyStates.insert(std::pair(Key::Space, false));
		keyStates.insert(std::pair(Key::Apostrophe, false));
		keyStates.insert(std::pair(Key::Comma, false));
		keyStates.insert(std::pair(Key::Minus, false));
		keyStates.insert(std::pair(Key::Period, false));
		keyStates.insert(std::pair(Key::Slash, false));

		keyStates.insert(std::pair(Key::D0, false));
		keyStates.insert(std::pair(Key::D1, false));
		keyStates.insert(std::pair(Key::D2, false));
		keyStates.insert(std::pair(Key::D3, false));
		keyStates.insert(std::pair(Key::D4, false));
		keyStates.insert(std::pair(Key::D5, false));
		keyStates.insert(std::pair(Key::D6, false));
		keyStates.insert(std::pair(Key::D7, false));
		keyStates.insert(std::pair(Key::D8, false));
		keyStates.insert(std::pair(Key::D9, false));

		keyStates.insert(std::pair(Key::Semicolon, false));
		keyStates.insert(std::pair(Key::Equal, false));

		keyStates.insert(std::pair(Key::A, false));
		keyStates.insert(std::pair(Key::B, false));
		keyStates.insert(std::pair(Key::C, false));
		keyStates.insert(std::pair(Key::D, false));
		keyStates.insert(std::pair(Key::E, false));
		keyStates.insert(std::pair(Key::F, false));
		keyStates.insert(std::pair(Key::G, false));
		keyStates.insert(std::pair(Key::H, false));
		keyStates.insert(std::pair(Key::I, false));
		keyStates.insert(std::pair(Key::J, false));
		keyStates.insert(std::pair(Key::K, false));
		keyStates.insert(std::pair(Key::L, false));
		keyStates.insert(std::pair(Key::M, false));
		keyStates.insert(std::pair(Key::N, false));
		keyStates.insert(std::pair(Key::O, false));
		keyStates.insert(std::pair(Key::P, false));
		keyStates.insert(std::pair(Key::Q, false));
		keyStates.insert(std::pair(Key::R, false));
		keyStates.insert(std::pair(Key::S, false));
		keyStates.insert(std::pair(Key::T, false));
		keyStates.insert(std::pair(Key::U, false));
		keyStates.insert(std::pair(Key::V, false));
		keyStates.insert(std::pair(Key::W, false));
		keyStates.insert(std::pair(Key::X, false));
		keyStates.insert(std::pair(Key::Y, false));
		keyStates.insert(std::pair(Key::Z, false));

		keyStates.insert(std::pair(Key::LeftBracket, false));
		keyStates.insert(std::pair(Key::Backslash, false));
		keyStates.insert(std::pair(Key::RightBracket, false));
		keyStates.insert(std::pair(Key::GraveAccent, false));

		keyStates.insert(std::pair(Key::World1, false));
		keyStates.insert(std::pair(Key::World2, false));

		keyStates.insert(std::pair(Key::Escape, false));
		keyStates.insert(std::pair(Key::Enter, false));
		keyStates.insert(std::pair(Key::Tab, false));
		keyStates.insert(std::pair(Key::Backspace, false));
		keyStates.insert(std::pair(Key::Insert, false));
		keyStates.insert(std::pair(Key::Delete, false));
		keyStates.insert(std::pair(Key::Right, false));
		keyStates.insert(std::pair(Key::Left, false));
		keyStates.insert(std::pair(Key::Down, false));
		keyStates.insert(std::pair(Key::Up, false));
		keyStates.insert(std::pair(Key::PageUp, false));
		keyStates.insert(std::pair(Key::PageDown, false));
		keyStates.insert(std::pair(Key::Home, false));
		keyStates.insert(std::pair(Key::End, false));
		keyStates.insert(std::pair(Key::CapsLock, false));
		keyStates.insert(std::pair(Key::ScrollLock, false));
		keyStates.insert(std::pair(Key::NumLock, false));
		keyStates.insert(std::pair(Key::PrintScreen, false));
		keyStates.insert(std::pair(Key::Pause, false));

		keyStates.insert(std::pair(Key::F1, false));
		keyStates.insert(std::pair(Key::F2, false));
		keyStates.insert(std::pair(Key::F3, false));
		keyStates.insert(std::pair(Key::F4, false));
		keyStates.insert(std::pair(Key::F5, false));
		keyStates.insert(std::pair(Key::F6, false));
		keyStates.insert(std::pair(Key::F7, false));
		keyStates.insert(std::pair(Key::F8, false));
		keyStates.insert(std::pair(Key::F9, false));
		keyStates.insert(std::pair(Key::F10, false));
		keyStates.insert(std::pair(Key::F11, false));
		keyStates.insert(std::pair(Key::F12, false));
		keyStates.insert(std::pair(Key::F13, false));
		keyStates.insert(std::pair(Key::F14, false));
		keyStates.insert(std::pair(Key::F15, false));
		keyStates.insert(std::pair(Key::F16, false));
		keyStates.insert(std::pair(Key::F17, false));
		keyStates.insert(std::pair(Key::F18, false));
		keyStates.insert(std::pair(Key::F19, false));
		keyStates.insert(std::pair(Key::F20, false));
		keyStates.insert(std::pair(Key::F21, false));
		keyStates.insert(std::pair(Key::F22, false));
		keyStates.insert(std::pair(Key::F23, false));
		keyStates.insert(std::pair(Key::F24, false));
		keyStates.insert(std::pair(Key::F25, false));

		keyStates.insert(std::pair(Key::KP0, false));
		keyStates.insert(std::pair(Key::KP1, false));
		keyStates.insert(std::pair(Key::KP2, false));
		keyStates.insert(std::pair(Key::KP3, false));
		keyStates.insert(std::pair(Key::KP4, false));
		keyStates.insert(std::pair(Key::KP5, false));
		keyStates.insert(std::pair(Key::KP6, false));
		keyStates.insert(std::pair(Key::KP7, false));
		keyStates.insert(std::pair(Key::KP8, false));
		keyStates.insert(std::pair(Key::KP9, false));
		keyStates.insert(std::pair(Key::KPDecimal, false));
		keyStates.insert(std::pair(Key::KPDivide, false));
		keyStates.insert(std::pair(Key::KPMultiply, false));
		keyStates.insert(std::pair(Key::KPSubtract, false));
		keyStates.insert(std::pair(Key::KPAdd, false));
		keyStates.insert(std::pair(Key::KPEnter, false));
		keyStates.insert(std::pair(Key::KPEqual, false));

		keyStates.insert(std::pair(Key::LeftShift, false));
		keyStates.insert(std::pair(Key::LeftControl, false));
		keyStates.insert(std::pair(Key::LeftAlt, false));
		keyStates.insert(std::pair(Key::LeftSuper, false));
		keyStates.insert(std::pair(Key::RightShift, false));
		keyStates.insert(std::pair(Key::RightControl, false));
		keyStates.insert(std::pair(Key::RightAlt, false));
		keyStates.insert(std::pair(Key::RightSuper, false));
		keyStates.insert(std::pair(Key::Menu, false));

		//Inserting mouse states
		mouseStates.insert(std::pair(Mouse::Button0, false));
		mouseStates.insert(std::pair(Mouse::Button1, false));
		mouseStates.insert(std::pair(Mouse::Button2, false));
		mouseStates.insert(std::pair(Mouse::Button3, false));
		mouseStates.insert(std::pair(Mouse::Button4, false));
		mouseStates.insert(std::pair(Mouse::Button5, false));
		mouseStates.insert(std::pair(Mouse::Button6, false));
		mouseStates.insert(std::pair(Mouse::Button7, false));

		mouseStates.insert(std::pair(Mouse::ButtonLast, false));
		mouseStates.insert(std::pair(Mouse::ButtonLeft, false));
		mouseStates.insert(std::pair(Mouse::ButtonRight, false));
		mouseStates.insert(std::pair(Mouse::ButtonMiddle, false));
	}

	void Input::Shutdown()
	{
		//Does not do much at the moment as input is a purely static class, but this may change in future

		keyStates.clear();
		mouseStates.clear();
	}
}