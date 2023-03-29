//@BridgetACasey

#include "wzpch.h"
#include "ImGuiLayer.h"

//TEMPORARY INCLUDE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "audio/Audio.h"
#include "audio/AudioSource.h"
#include "base/ResourcePathFinder.h"
#include "common/Application.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "editor/ui/screenreading/ScreenReaderLogger.h"

namespace Wizzard
{
	static ImGuiWindow* currentHoveredWindow = nullptr;
	static ImGuiWindow* currentActiveWindow = nullptr;
	static ImGuiID currentHoveredID;
	static ImGuiID currentActiveID;

	static AudioSource windowChangeSFX;

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
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

		//Set scaling options, may do this in a different class for user preferences later
		//ImGuiSR::SetButtonFontScale(6.0f);

		//Apply scaling options
		ImFontConfig imFontConfig;
		imFontConfig.SizePixels = 13.0f * 6.0f;

		//ImGuiContext* context = ImGui::GetCurrentContext();
		//
		//ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiKey_Tab, ImGuiKeyOwner_None);
		//ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab, ImGuiKeyOwner_None);
		//
		//if (context->ConfigNavWindowingKeyNext)
		//	ImGui::SetShortcutRouting(context->ConfigNavWindowingKeyNext, ImGuiKeyOwner_None);
		//
		//if (context->ConfigNavWindowingKeyPrev)
		//	ImGui::SetShortcutRouting(context->ConfigNavWindowingKeyPrev, ImGuiKeyOwner_None);
		//
		//context->ConfigNavWindowingKeyNext = ImGuiKey_LeftCtrl;	//TODO: Find a way to remap this to a single key press, no key holds
		//context->ConfigNavWindowingKeyPrev = ImGuiMod_None | ImGuiKey_Enter;

		std::string fontPath = ResourcePath::GetResourcePath(FONT, "OpenSans-Bold.ttf");
		io.Fonts->AddFontFromFileTTF(fontPath.c_str(), imFontConfig.SizePixels, &imFontConfig);

		io.FontAllowUserScaling = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
			style.Colors[ImGuiCol_Tab] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.75f, 0.75f, 1.0f);
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.75f, 0.75f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.1f, 0.75f, 1.0f);
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());	//TODO: Make this window call platform agnostic

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);	//TODO: Make imgui implementation calls API agnostic
		ImGui_ImplOpenGL3_Init("#version 410");

		if(Audio::IsActive())
		{
			windowChangeSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "page-flip-01a.mp3"), false, true, WIZ_AUDIO_UIWINDOWCHANGED);
		}
	}

	void ImGuiLayer::OnDetach()
	{
		WIZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		windowChangeSFX.FreeSource();
	}

	void ImGuiLayer::Begin()
	{
		WIZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
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
		currentHoveredID = context->HoveredId;
		currentActiveID = context->ActiveId;

		/*
		 * Since dearimgui is an immediate mode GUI, there are no built-in event callbacks for hovering/selecting items.
		 * However, we need to know when this happens so we can trigger output to a screen reader, so using own engine callbacks instead.
		 */

		if (context->HoveredWindow != nullptr && currentHoveredWindow != context->HoveredWindow)
		{
			currentHoveredWindow = context->HoveredWindow;

			UIWindowHoveredEvent uiWindowEvent(currentHoveredWindow->ID, true);
			WIZ_INFO("ImGui Window Name: {0}, TABID: {1}", currentHoveredWindow->Name, currentHoveredWindow->TabId);
			OnUIWindowHoveredEvent(uiWindowEvent);
		}

		if (context->ActiveIdWindow != nullptr && currentActiveWindow != context->ActiveIdPreviousFrameWindow)
		{
			currentActiveWindow = context->ActiveIdWindow;

			UIWindowFocusEvent uiWindowEvent(currentActiveWindow->ID, true);
			WIZ_INFO("ImGui Window Name: {0}, TabID: {1}", currentActiveWindow->Name, currentActiveWindow->TabId);
			OnUIWindowFocusEvent(uiWindowEvent);
		}

		if (currentHoveredID != 0 && currentHoveredID != context->HoveredIdPreviousFrame)
		{
			UIElementHoveredEvent uiFocusEvent(currentHoveredID, true);
			OnUIElementHovered(uiFocusEvent);
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

		eventHandler.HandleEvent<UIWindowHoveredEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnUIWindowHoveredEvent));
		eventHandler.HandleEvent<UIWindowFocusEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnUIWindowFocusEvent));
		eventHandler.HandleEvent<UIElementHoveredEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnUIElementHovered));
		eventHandler.HandleEvent<UIElementSelectedEvent>(WIZ_BIND_EVENT_FN(ImGuiLayer::OnUIElementSelected));

		if(blockImGuiEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			event.isHandled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.isHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	bool ImGuiLayer::OnUIWindowHoveredEvent(UIWindowHoveredEvent& uiWindowHoveredEvent)
	{
		WIZ_TRACE(uiWindowHoveredEvent);

		uiWindowMessageID = uiWindowHoveredEvent.GetElementID();
		logWindowMessage = true;

		return false;
	}

	bool ImGuiLayer::OnUIWindowFocusEvent(UIWindowFocusEvent& uiWindowFocusEvent)
	{
		WIZ_TRACE(uiWindowFocusEvent);

		uiWindowMessageID = uiWindowFocusEvent.GetElementID();
		//logWindowMessage = true;

		//TODO: Still have to fix this playing over elements sometimes
		if(currentActiveWindow != nullptr && currentActiveID != currentActiveWindow->TabId)
		Audio::Play(windowChangeSFX);

		return false;
	}

	bool ImGuiLayer::OnUIElementHovered(UIElementHoveredEvent& uiElementHoveredEvent)
	{
		WIZ_TRACE(uiElementHoveredEvent);

		uiElementMessageID = uiElementHoveredEvent.GetElementID();
		logElementMessage = true;

		//if(uiElementHoveredEvent.GetElementID() == )

		return false;
	}

	bool ImGuiLayer::OnUIElementSelected(UIElementSelectedEvent& uiElementSelectedEvent)
	{
		WIZ_TRACE(uiElementSelectedEvent);

		uiElementMessageID = uiElementSelectedEvent.GetElementID();
		//logElementMessage = true;

		ScreenReaderLogger::Stop();

		return false;
	}
}
