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

#include "base/ResourcePathFinder.h"
#include "core/Application.h"
#include "ImGuiScreenReading.h"

namespace Wizzard
{
//TODO: Use lambda instead of std::bind perhaps?
#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)

	static ImGuiWindow* currentWindow = nullptr;
	static ImGuiID currentActiveID;
	static ImGuiID currentHoveredID;

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

		imguiIniFilePath = ResourcePath::GetResourcePath(CONFIG, "imgui.ini");
		io.IniFilename = imguiIniFilePath.c_str();

		imguiLogFilePath = ResourcePath::GetResourcePath(CONFIG, "imgui_log.txt");
		io.LogFilename = imguiLogFilePath.c_str();

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

		std::string fontPath = ResourcePath::GetResourcePath(FONT, "OpenSans-Bold.ttf");
		io.Fonts->AddFontFromFileTTF(fontPath.c_str(), imFontConfig.SizePixels, &imFontConfig);

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
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.75f, 0.75f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.1f, 0.75f, 1.0f);
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
		currentWindow = context->ActiveIdWindow;
		currentHoveredID = context->HoveredId;
		currentActiveID = context->ActiveId;

		/*
		 * Since dearimgui is an immediate mode GUI, there are no built-in event callbacks for hovering/selecting items.
		 * However, we need to know when this happens so we can trigger output to a screen reader, so using own engine callbacks instead.
		 */

		if (currentWindow != nullptr && currentWindow != context->ActiveIdPreviousFrameWindow)
		{
			UIWindowFocusEvent uiWindowEvent(currentWindow->ID, true);
			OnUIWindowFocusEvent(uiWindowEvent);
		}

		if (currentHoveredID != 0 && currentHoveredID != context->HoveredIdPreviousFrame)
		{
			UIElementFocusEvent uiFocusEvent(currentHoveredID, true);
			OnUIElementFocus(uiFocusEvent);
		}

		if (currentActiveID != 0 && currentActiveID != context->ActiveIdPreviousFrame)
		{
			UIElementSelectedEvent uiSelectEvent(currentActiveID, true);
			OnUIElementSelected(uiSelectEvent);
		}
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		WIZ_PROFILE_FUNCTION();

		EventHandler eventHandler(event);

		eventHandler.HandleEvent<UIWindowFocusEvent>(BIND_EVENT_FN(ImGuiLayer::OnUIWindowFocusEvent));
		eventHandler.HandleEvent<UIElementFocusEvent>(BIND_EVENT_FN(ImGuiLayer::OnUIElementFocus));
		eventHandler.HandleEvent<UIElementSelectedEvent>(BIND_EVENT_FN(ImGuiLayer::OnUIElementSelected));

		if(blockImGuiEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			event.isHandled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.isHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	bool ImGuiLayer::OnUIWindowFocusEvent(UIWindowFocusEvent& uiWindowFocusEvent)
	{
		WIZ_TRACE(uiWindowFocusEvent);

		return false;
	}

	bool ImGuiLayer::OnUIElementFocus(UIElementFocusEvent& uiElementFocusEvent)
	{
		WIZ_TRACE(uiElementFocusEvent);

		return false;
	}

	bool ImGuiLayer::OnUIElementSelected(UIElementSelectedEvent& uiElementSelectedEvent)
	{
		WIZ_TRACE(uiElementSelectedEvent);

		return false;
	}
}