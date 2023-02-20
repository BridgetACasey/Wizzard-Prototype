//@BridgetACasey

#include "wzpch.h"
#include "ImGuiLayer.h"

//TEMPORARY INCLUDE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "imgui.h"
#include "ImGuiCustom.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "core/Application.h"

#include "ImGuiScreenReading.h"

static ImGuiWindow* currentWindow = nullptr;
static ImGuiID currentActiveID;
static ImGuiID currentHoveredID;

namespace Wizzard
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
		WIZ_PROFILE_FUNCTION();
	}

	ImGuiLayer::~ImGuiLayer()
	{
		WIZ_PROFILE_FUNCTION();
	}

	void ImGuiLayer::OnAttach()
	{
		WIZ_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

		//Set scaling options, may do this in a different class for user preferences later
		ImGuiSR::SetButtonFontScale(6.0f);

		//Apply scaling options
		ImFontConfig imFontConfig;
		imFontConfig.SizePixels = 13.0f * ImGuiSR::GetButtonFontScale();
		io.Fonts->AddFontFromFileTTF("OpenSans-Bold.ttf", imFontConfig.SizePixels, &imFontConfig);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		WIZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		WIZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		WIZ_PROFILE_FUNCTION();

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
		ImGuiContext* context = ImGui::GetCurrentContext();
		currentActiveID = context->ActiveId;
		currentHoveredID = context->HoveredId;

		if (currentActiveID != 0 && currentActiveID != context->ActiveIdPreviousFrame)
		{
			WIZ_TRACE("Yoooo we've swapped active thingies! Thingie: {0}", currentActiveID);
		}

		if (currentHoveredID != 0 && currentHoveredID != context->HoveredIdPreviousFrame)
		{
			WIZ_TRACE("Yoooo we've swapped hovered thingies! Thingie: {0}", currentHoveredID);
		}
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		WIZ_PROFILE_FUNCTION();

		if(blockImGuiEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			event.isHandled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.isHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}
}