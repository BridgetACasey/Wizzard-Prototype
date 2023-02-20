//@BridgetACasey

#include "EditorLayer.h"

#include "imgui/imgui.h"

namespace Wizzard
{
	EditorLayer::EditorLayer() : Layer("Editor"), orthoCamController(1920.0f / 1080.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		customTexture = Texture2D::Create("res/textures/smiley.png");

		music = AudioSource::LoadFromFile("res/music/examplemusic.mp3", false);

		FramebufferSpecification fbSpec;
		fbSpec.width = 1920;
		fbSpec.height = 1080;
		frameBuffer = Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		music.FreeSource();
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		if (isViewportFocused)
		{
			orthoCamController.OnUpdate(timeStep);

			//Temp test code for audio demo
			if (Input::IsKeyPressed(Key::P))
			{
				LUG_TRACE("Play key pressed! Hopefully just once???");

				playMusic = !playMusic;

				if (music.IsLoaded())
				{
					if (playMusic)
						Audio::Play(music);
					else
						Audio::Pause(music);
				}
			}
		}

		// Render
		Renderer2D::ResetStatistics();

		frameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += timeStep * 50.0f;

		Renderer2D::BeginScene(orthoCamController.GetCamera());
		Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, customTexture, 10.0f);
		Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, customTexture, 1.0f);
		Renderer2D::EndScene();

		frameBuffer->Unbind();

	}

	void EditorLayer::OnImGuiRender()
	{
		WIZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;


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

		if (Input::IsKeyDown(Key::Z))
		{
			ImGui::SetNextWindowFocus();
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMainMenuBar())
		{
			float windowWidth = (float)Application::Get().GetWindow().GetWidth() / 6.15f;

			isMainMenuFocused = ImGui::IsWindowFocused();
			isMainMenuHovered = ImGui::IsWindowHovered();

			if (ImGuiSR::Button("FILE", L"file menu", ImVec2(windowWidth, 80.5f)))
			{
				ImGui::SetItemDefaultFocus();
				openFileMenu = !openFileMenu;
			}

			if (ImGuiSR::Button("SCENE", L"scene menu", ImVec2(windowWidth, 80.5f)))
				openEditMenu = !openEditMenu;

			if (ImGuiSR::Button("CREATE", L"create menu", ImVec2(windowWidth, 80.5f)))
				openObjectMenu = !openObjectMenu;

			if (ImGuiSR::Button("CONSOLE", L"console menu", ImVec2(windowWidth, 80.5f)))
				openConsoleMenu = !openConsoleMenu;

			if (ImGuiSR::Button("SETTINGS", L"settings menu", ImVec2(windowWidth, 80.5f)))
				openSettingsMenu = !openSettingsMenu;

			if (ImGuiSR::Button("EXIT", L"exit menu", ImVec2(windowWidth, 80.5f)))
				openExitMenu = !openExitMenu;

			ImGui::EndMainMenuBar();
		}

			if (Input::IsKeyDown(Key::X))
			{
				ImGui::SetNextWindowFocus();
			}

			ImGui::Begin("Hierarchy");

			isHierarchyFocused = ImGui::IsWindowFocused();
			isHierarchyHovered = ImGui::IsWindowHovered();

			ImGuiSR::Button("Test Button", L"This is the hierarchy test button", ImVec2(150.0f, 150.0f));

			if(openFileMenu)
			{
				ImGui::TextWrapped("Project functions like saving go here!");
			}
			else if (openEditMenu)
			{
				ImGui::TextWrapped("Editing functions found here!");
			}
			else if (openObjectMenu)
			{
				ImGui::TextWrapped("Scene hierarchy data found here!");
			}
			else if (openConsoleMenu)
			{
				//ImGui::TextWrapped("Debug information goes here!");

				auto stats = Renderer2D::GetStatistics();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			}
			else if (openSettingsMenu)
			{
				ImGui::TextWrapped("User preferences go here!");
			}

			if(openExitMenu)
			{
				if (ImGui::Begin("Exit Menu"))
				{
					ImGui::Text("Quit application?");

					if (ImGuiSR::Button("Yes", L"Yes", ImVec2(240.0f, 80.0f)))
					{
						Application::Get().Close();
					}

					ImGui::SameLine();

					if (ImGuiSR::Button("No", L"No", ImVec2(240.0f, 80.0f)))
						openExitMenu = false;

					ImGui::End();
				}
			}

			ImGui::End();	//End Hierarchy

			if (Input::IsKeyDown(Key::C))
			{
				ImGui::SetNextWindowFocus();
			}

			static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("Viewport", &dockspaceOpen, viewportFlags);

			ImGuiSR::Button("Test Button", L"Viewport test button", ImVec2(150.0f, 150.0f));

			isViewportFocused = ImGui::IsWindowFocused();
			isViewportHovered = ImGui::IsWindowHovered();

			Application::Get().GetImGuiLayer()->BlockImGuiEvents(!isViewportFocused || !isViewportHovered);

			uint32_t textureID = frameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 1920, 1080 }, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();	//End Viewport

		ImGui::End();	//End Dockspace Demo
	}

	void EditorLayer::OnEvent(Event& event)
	{
		orthoCamController.OnEvent(event);
	}
}