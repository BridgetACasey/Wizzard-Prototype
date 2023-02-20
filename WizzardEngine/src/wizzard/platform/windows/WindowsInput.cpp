//@BridgetACasey

#include "wzpch.h"

#include "input/Input.h"
#include "wizzard/core/Application.h"
#include "GLFW/glfw3.h"

namespace Wizzard
{
	constexpr int TOTAL_KEY_CODES = Key::TotalCodeCount;
	constexpr int TOTAL_MOUSE_CODES = Mouse::TotalCodeCount;

	//Storing the states for each unique key and mouse button to check if it has only been pressed once
	static std::unordered_map<int, bool> keyStates;
	static std::unordered_map<int, bool> mouseStates;

	void Input::Init()
	{
		for (int i = 0; i < TOTAL_KEY_CODES; i++)
			keyStates.insert(std::pair(i, false));

		for (int j = 0; j < TOTAL_MOUSE_CODES; j++)
			mouseStates.insert(std::pair(j, false));
	}

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

	bool Input::IsKeyDown(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		if (state == GLFW_RELEASE)
			keyStates.at(keyCode) = false;

		const bool pressed = keyStates.at(keyCode);

		return pressed;
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

	bool Input::IsMouseButtonDown(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonReleased(MouseCode mouseCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_RELEASE;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double posX, posY;

		glfwGetCursorPos(window, &posX, &posY);

		return { (float)posX, (float)posY };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}
}
