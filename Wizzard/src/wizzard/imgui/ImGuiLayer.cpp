//@BridgetACasey

#include "wzpch.h"
#include "ImGuiLayer.h"

#include "core/Application.h"

//TEMPORARY INCLUDE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

namespace Wizzard
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		//ImGui_ImplOpenGL3_Init("#version 410");
		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

		float t = static_cast<float>(glfwGetTime());

		io.DeltaTime = time > 0.0f ? (t - time) : (1.0f / 60.0f);
		time = t;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		//EventHandler dispatcher(event);
		//
		//dispatcher.HandleEvent<MouseButtonPressedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		//dispatcher.HandleEvent<MouseButtonReleasedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		//dispatcher.HandleEvent<MouseMovedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		//dispatcher.HandleEvent<MouseScrolledEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		//dispatcher.HandleEvent<KeyPressedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		//dispatcher.HandleEvent<KeyTypedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		//dispatcher.HandleEvent<KeyReleasedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		//dispatcher.HandleEvent<WindowResizeEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	//bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& mouseEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.MouseDown[mouseEvent.GetMouseButton()] = true;
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& mouseEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.MouseDown[mouseEvent.GetMouseButton()] = false;
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& mouseEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.MousePos = ImVec2(mouseEvent.GetX(), mouseEvent.GetY());
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& mouseEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.MouseWheelH += mouseEvent.GetXOffset();
	//	io.MouseWheel += mouseEvent.GetYOffset();
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& keyEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.KeysDown[keyEvent.GetKeyCode()] = true;
	//
	//	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& keyEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.KeysDown[keyEvent.GetKeyCode()] = false;
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& keyEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	int keycode = keyEvent.GetKeyCode();
	//
	//	if (keycode > 0 && keycode < 0x10000)
	//		io.AddInputCharacter((unsigned short)keycode);
	//
	//	return false;
	//}
	//
	//bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& windowEvent)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//
	//	io.DisplaySize = ImVec2(windowEvent.GetWidth(), windowEvent.GetHeight());
	//	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	//
	//	glViewport(0, 0, windowEvent.GetWidth(), windowEvent.GetHeight());
	//
	//	return false;
	//}
}