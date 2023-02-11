//@BridgetACasey

#include "wzpch.h"
#include "ImGuiLayer.h"

//TEMPORARY INCLUDE
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "core/Application.h"

#include "ImGuiAccessibility.h"

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

		//Set scaling options, may do this in a different class for user preferences later
		ImGuiAccessibility::SetButtonFontScale(6.0f);

		//Apply scaling options
		ImFontConfig imFontConfig;
		imFontConfig.SizePixels = 13.0f * ImGuiAccessibility::GetButtonFontScale();
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
			style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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
		Begin();

		//TEMP CODE FOR TESTING & DEMO PURPOSES ONLY
		//UI will not be handled like this in final application
		//It's also a bit buggy when clicking on menu items/dropdowns but acceptable for demo only

		static bool read = true;

		//Temporary value for opening menus
		static bool openFileMenu = false;
		static bool openEditMenu = false;
		static bool openObjectMenu = false;
		static bool openConsoleMenu = false;
		static bool openSettingsMenu = false;
		static bool openExitMenu = false;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGuiAccessibility::Button("File", L"File Tab", read, ImVec2(311.0f, 80.5f)))
				openFileMenu = !openFileMenu;

			if (ImGuiAccessibility::Button("Edit", L"Edit Tab", read, ImVec2(311.0f, 80.5f)))
				openEditMenu = !openEditMenu;

			if (ImGuiAccessibility::Button("Object", L"Object Tab", read, ImVec2(311.0f, 80.5f)))
				openObjectMenu = !openObjectMenu;

			if (ImGuiAccessibility::Button("Console", L"Console Tab", read, ImVec2(311.0f, 80.5f)))
				openConsoleMenu = !openConsoleMenu;

			if (ImGuiAccessibility::Button("Settings", L"Settings Tab", read, ImVec2(311.0f, 80.5f)))
				openSettingsMenu = !openSettingsMenu;

			if (ImGuiAccessibility::Button("Exit", L"Exit Tab", read, ImVec2(311.0f, 80.5f)))
				openExitMenu = !openExitMenu;

			ImGui::EndMainMenuBar();
		}

		if (openFileMenu)
		{
			if (ImGui::Begin("File Menu"))
			{
				ImGui::TextWrapped("Project functions like saving go here!");
				ImGui::End();
			}
		}

		if (openEditMenu)
		{
			if (ImGui::Begin("Edit Menu"))
			{
				ImGui::TextWrapped("Editing functions found here!");
				ImGui::End();
			}
		}

		if (openObjectMenu)
		{
			if (ImGui::Begin("Object Menu"))
			{
				ImGui::TextWrapped("Scene hierarchy data found here!");
				ImGui::End();
			}
		}

		if (openConsoleMenu)
		{
			if (ImGui::Begin("Console Menu"))
			{
				ImGui::TextWrapped("Debug information goes here!");
				ImGui::End();
			}
		}

		if (openSettingsMenu)
		{
			if (ImGui::Begin("Settings Menu"))
			{
				ImGui::TextWrapped("User preferences go here!");
				ImGui::End();
			}
		}

		if (openExitMenu)
		{
			if (ImGui::Begin("Exit Menu"))
			{
				ImGui::Text("Quit application?");

				if (ImGuiAccessibility::Button("Yes", L"Yes", read, ImVec2(240.0f, 80.0f)))
				{
					Application::Get().Close();
				}

				ImGui::SameLine();

				if (ImGuiAccessibility::Button("No", L"No", read, ImVec2(240.0f, 80.0f)))
					openExitMenu = false;

				ImGui::End();
			}
		}

		End();
	}
}