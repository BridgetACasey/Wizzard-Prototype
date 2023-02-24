//@BridgetACasey

#include "EditorLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "wizzard/scene/SceneSerialiser.h"

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

		activeScene = CreateRef<Scene>();

		// Entity
		auto square = activeScene->CreateEntity("Green Square");
		square.AddComponent<SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		square.AddComponent<RigidBody2DComponent>();

		m_SquareEntity = square;

		m_CameraEntity = activeScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = activeScene->CreateEntity("Clip-Space Entity");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		sceneHierarchyPanel.SetContext(activeScene);

		activeScene->OnStart();
	}

	void EditorLayer::OnDetach()
	{
		music.FreeSource();
		activeScene->OnStop();
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		// Resize
		if (FramebufferSpecification spec = frameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
		{
			frameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			orthoCamController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			activeScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		//if (isViewportFocused)
		//{
		//	orthoCamController.OnUpdate(timeStep);
		//
		//	//Temp test code for audio demo
		//	if (Input::IsKeyReleased(Key::P))
		//	{
		//		LUG_TRACE("Playing example music.");
		//
		//		playMusic = !playMusic;
		//
		//		if (music.IsLoaded())
		//		{
		//			if (playMusic)
		//				Audio::Play(music);
		//			else
		//				Audio::Pause(music);
		//		}
		//	}
		//
		//	if (Input::IsKeyPressed(Key::T))
		//	{
		//		LUG_TRACE("Mouse go weeeeee! {0}", Input::GetInputStateAsString(Input::GetKeyState(Key::T)));
		//		Input::SetMousePosition(Input::GetMousePositionX() + 10.0f, Input::GetMousePositionY());
		//	}
		//}

		if (Input::IsKeyPressed(Key::Z))
		{
			LUG_TRACE("Attempting to detect screen reader at runtime.");
			ScreenReaderSupport::DetectScreenReader();
		}

		if (Input::IsKeyPressed(Key::F5))
		{
			SceneSerialiser serializer(activeScene);
			serializer.Serialise("res/scenes/Example.wizzard");
		}

		if (Input::IsKeyPressed(Key::F9))
		{
			SceneSerialiser serializer(activeScene);
			serializer.Deserialise("res/scenes/Example.wizzard");
		}

		// Render
		Renderer2D::ResetStatistics();

		frameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		static float rotation = 0.0f;
		rotation += timeStep * 50.0f;

		activeScene->OnUpdate(timeStep);

		//sceneHierarchyPanel.OnImGuiRender();

		//Renderer2D::BeginScene(orthoCamController.GetCamera());
		//Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, customTexture, 10.0f);
		//Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, customTexture, 1.0f);
		//Renderer2D::EndScene();

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
		static bool openFileMenu = false;
		static bool openEditMenu = false;
		static bool openObjectMenu = false;
		static bool openConsoleMenu = false;
		static bool openSettingsMenu = false;
		static bool openExitMenu = false;

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

			if (ImGuiSR::Button("FILE", ImVec2(windowWidth, 80.5f), "file menu"))
			{
				ImGui::SetItemDefaultFocus();
				openFileMenu = !openFileMenu;
			}

			if (ImGuiSR::Button("SCENE", ImVec2(windowWidth, 80.5f), "scene menu"))
				openEditMenu = !openEditMenu;

			if (ImGuiSR::Button("CREATE", ImVec2(windowWidth, 80.5f), "create menu"))
				openObjectMenu = !openObjectMenu;

			if (ImGuiSR::Button("CONSOLE", ImVec2(windowWidth, 80.5f), "console menu"))
				openConsoleMenu = !openConsoleMenu;

			if (ImGuiSR::Button("SETTINGS", ImVec2(windowWidth, 80.5f), "settings menu"))
				openSettingsMenu = !openSettingsMenu;

			if (ImGuiSR::Button("EXIT", ImVec2(windowWidth, 80.5f), "exit menu"))
				openExitMenu = !openExitMenu;

			ImGui::EndMainMenuBar();
		}

			ImGui::Begin("Hierarchy");

			if (m_SquareEntity)
			{
				ImGui::Separator();
				auto& tag = m_SquareEntity.GetComponent<TagComponent>().tag;
				ImGui::Text("%s", tag.c_str());

				auto& squareColor = m_SquareEntity.GetComponent<SpriteComponent>().color;
				ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
				ImGui::Separator();
			}

			ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().GetTransform()[3]));

			if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
			{
				m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
				m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
			}

			{
				auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);
			}

			isHierarchyFocused = ImGui::IsWindowFocused();
			isHierarchyHovered = ImGui::IsWindowHovered();

			ImGuiSR::Button("Test Button", ImVec2(150.0f, 150.0f), "This is the hierarchy test button.", true);

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
				auto stats = Renderer2D::GetStatistics();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.drawCalls);
				ImGui::Text("Quads: %d", stats.quadCount);
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

					if (ImGuiSR::Button("Yes", ImVec2(240.0f, 80.0f)))
					{
						//TODO: Fix message being cut off by exiting app before speech has finished
						ScreenReaderSupport::OutputAll("Exiting editor application.");

						if(!ScreenReaderSupport::IsSpeaking())
							Application::Get().Close();
					}

					ImGui::SameLine();

					if (ImGuiSR::Button("No", ImVec2(240.0f, 80.0f)))
						openExitMenu = false;

					ImGui::End();
				}
			}

			ImGui::End();	//End Hierarchy

			static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("Viewport", &dockspaceOpen, viewportFlags);

			ImGuiSR::Button("Test Button", ImVec2(150.0f, 150.0f), "Hey bestie, this is the viewport test button.", true);

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