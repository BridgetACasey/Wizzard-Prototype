//@BridgetACasey

#include "wzpch.h"
#include "WindowsInput.h"

#include "wizzard/core/Application.h"
#include "GLFW/glfw3.h"

namespace Wizzard
{
	Input* Input::instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(KeyCodeGL keycode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseCodeGL button)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double posX, posY;

		glfwGetCursorPos(window, &posX, &posY);

		return { (float)posX, (float)posY };
	}

	float WindowsInput::GetMouseXImpl()
	{
		WIZ_PROFILE_FUNCTION();

		auto [x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		WIZ_PROFILE_FUNCTION();

		auto [x, y] = GetMousePositionImpl();

		return y;
	}
}