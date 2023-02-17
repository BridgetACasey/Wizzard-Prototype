//@BridgetACasey

#include "wzpch.h"

#include "input/Input.h"
#include "wizzard/core/Application.h"
#include "GLFW/glfw3.h"

namespace Wizzard
{
	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS;
	}

	bool Input::IsKeyHeld(KeyCode keyCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(MouseCode mouseCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonHeld(MouseCode mouseCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonReleased(MouseCode mouseCode)
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, mouseCode);

		return state == GLFW_RELEASE;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		WIZ_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double posX, posY;

		glfwGetCursorPos(window, &posX, &posY);

		return { (float)posX, (float)posY };
	}

	float Input::GetMouseX()
	{
		WIZ_PROFILE_FUNCTION();

		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		WIZ_PROFILE_FUNCTION();

		auto [x, y] = GetMousePosition();

		return y;
	}
}
