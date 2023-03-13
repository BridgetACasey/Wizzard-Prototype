//@BridgetACasey

#include <wzpch.h>

#include "EditorLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "ImGuizmo.h"
#include "common/Application.h"
#include "input/Input.h"
#include "rendering/RenderCommand.h"
#include "rendering/Renderer2D.h"
#include "scene/component/CameraComponent.h"
#include "scene/component/SpriteComponent.h"
#include "editor/ui/screenreading/ScreenReaderSupport.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

#include "wizzard/base/ResourcePathFinder.h"
#include "wizzard/base/Maths.h"
#include "wizzard/scene/SceneSerialiser.h"
#include <scene/component/TransformComponent.h>

#include "scene/component/BoxCollider2DComponent.h"
#include "scene/component/RigidBody2DComponent.h"
#include "scene/component/TagComponent.h"

namespace Wizzard
{
	EditorLayer::EditorLayer() : Layer("Editor"), orthoCamController(1920.0f / 1080.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		customTexture = Texture2D::Create(ResourcePath::GetResourcePath(TEXTURE, "smiley.png"));

		music = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(MUSIC, "examplemusic.mp3"), false);

		FramebufferSpecification fbSpec;
		fbSpec.width = 1920;
		fbSpec.height = 1080;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		frameBuffer = Framebuffer::Create(fbSpec);

		editorScene = CreateRef<Scene>();
		activeScene = editorScene;

		cameraEntity = activeScene->CreateEntity("Scene Camera");
		cameraEntity.AddComponent<CameraComponent>();
		
		// Entity
		auto square = activeScene->CreateEntity("Green Square");
		square.AddComponent<SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		square.AddComponent<RigidBody2DComponent>();
		square.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Dynamic;
		square.AddComponent<BoxCollider2DComponent>();
		
		squareEntity = square;

		appSettingsPanel.SetSceneContext(activeScene);
		sceneHierarchyPanel.SetSceneContext(activeScene);
		objCreatePanel.SetSceneContext(activeScene);
		objPropertiesPanel.SetSceneContext(activeScene);

		editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	}

	void EditorLayer::OnDetach()
	{
		music.FreeSource();
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		activeScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

		// Resize
		if (FramebufferSpecification spec = frameBuffer->GetSpecification();
			viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != viewportSize.x || spec.height != viewportSize.y))
		{
			frameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			orthoCamController.OnResize(viewportSize.x, viewportSize.y);
			editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
		}

		if (Input::IsKeyPressed(Key::Z))
		{
			WIZ_TRACE("Attempting to detect screen reader at runtime.");
			ScreenReaderSupport::DetectScreenReader();
		}

		if (Input::IsMouseButtonPressed(Mouse::LeftButton))
		{
			if (isViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				sceneHierarchyPanel.SetSelectedEntity(hoveredEntity);
		}

		if (!ImGuizmo::IsUsing())
		{
			if (Input::IsKeyPressed(Key::W))
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			if (Input::IsKeyPressed(Key::E))
				gizmoType = ImGuizmo::OPERATION::ROTATE;
			if (Input::IsKeyPressed(Key::R))
				gizmoType = ImGuizmo::OPERATION::SCALE;
		}

		// Render
		Renderer2D::ResetStatistics();

		frameBuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		frameBuffer->ClearAttachment(1, -1);

		switch (activeScene->GetState())
		{
			case SceneState::EDIT:
			{
				if (isViewportFocused)
					orthoCamController.OnUpdate(timeStep);

				editorCamera.OnUpdate(timeStep);

				activeScene->OnUpdateEditor(timeStep, editorCamera);
				break;
			}
			case SceneState::PLAY:
			{
				activeScene->OnUpdatePlay(timeStep);
				break;
			}
			case SceneState::PAUSED:
				activeScene->Step();
				break;
			default:
				WIZ_ERROR("Scene State not yet implemented!");
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		glm::vec2 viewSize = viewportSize;
		my = viewSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewSize.x && mouseY < (int)viewSize.y)
		{
			int pixelData = frameBuffer->ReadPixel(1, mouseX, mouseY);
			hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, activeScene.get());
		}

		OnOverlayRender();

		frameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		WIZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

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
		static bool openFileMenu = true;
		static bool openEditMenu = false;
		static bool openObjectMenu = false;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

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

			if (ImGuiSR::Button("PROJECT", ImVec2(windowWidth, 80.5f)))
			{
				ImGui::SetItemDefaultFocus();
				openFileMenu = !openFileMenu;
			}

			if (ImGuiSR::Button("SCENE", ImVec2(windowWidth, 80.5f)))
				openEditMenu = !openEditMenu;

			if (ImGuiSR::Button("CREATE", ImVec2(windowWidth, 80.5f)))
				openObjectMenu = !openObjectMenu;

			//Temp
			static std::string playButtonLabel = "PLAY";
			static std::string playButtonDesc = "Play scene.";

			if (ImGuiSR::Button(playButtonLabel, ImVec2(windowWidth, 80.5f), playButtonDesc, true))
			{
				switch(activeScene->GetState())
				{
					case SceneState::EDIT:
					case SceneState::PAUSED:
					{
						OnSceneBeginPlay();
						playButtonLabel = "PAUSE";
						playButtonDesc = "Pause scene.";
					}
						break;
					case SceneState::PLAY:
					{
						OnScenePausePlay();
						playButtonLabel = "PLAY";
						playButtonDesc = "Play scene.";
					}
				}
			}

			if (ImGuiSR::Button("STOP", ImVec2(windowWidth, 80.5f), "Stop scene.", true))
			{
				if (activeScene->GetState() == SceneState::PLAY || activeScene->GetState() == SceneState::PAUSED)
				{
					OnSceneEndPlay();
					playButtonLabel = "PLAY";
					playButtonDesc = "Play scene.";
				}
			}

			ImGui::EndMainMenuBar();
		}

		if (openFileMenu)
			appSettingsPanel.OnImGuiRender();

		if (openEditMenu)
		{
			sceneHierarchyPanel.OnImGuiRender();
			//objPropertiesPanel.OnImGuiRender();
		}

		if (openObjectMenu)
			objCreatePanel.OnImGuiRender();


		//-----RENDERING THE VIEWPORT-----

			static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

			ImGuiSR::WindowBegin("Viewport", nullptr, viewportFlags);

			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			isViewportFocused = ImGui::IsWindowFocused();
			isViewportHovered = ImGui::IsWindowHovered();

			Application::Get().GetImGuiLayer()->BlockImGuiEvents(!isViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint64_t textureID = frameBuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// Gizmos
			Entity selectedEntity = sceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && gizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

				// Editor camera
				const glm::mat4& cameraProjection = editorCamera.GetProjection();
				glm::mat4 cameraView = editorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (gizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Maths::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}

			ImGui::End();	//End Viewport

		ImGui::End();	//End Dockspace Demo
	}

	void EditorLayer::OnEvent(Event& event)
	{
		orthoCamController.OnEvent(event);
		editorCamera.OnEvent(event);
	}

	void EditorLayer::OnOverlayRender() const
	{
		if (activeScene->GetState() == SceneState::PLAY)
		{
			Entity camera = activeScene->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
			Renderer2D::BeginScene(editorCamera);

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
	}

	bool EditorLayer::OpenProject()
	{
		return false;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
	}

	void EditorLayer::SaveProject()
	{
	}

	void EditorLayer::SaveProjectAs()
	{
	}

	void EditorLayer::NewScene()
	{
	}

	void EditorLayer::OpenScene()
	{
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
	}

	void EditorLayer::SaveScene()
	{
	}

	void EditorLayer::SaveSceneAs()
	{
	}

	void EditorLayer::OnSceneBeginPlay()
	{
		activeScene->SetState(SceneState::PLAY);

		activeScene = Scene::Copy(editorScene);
		activeScene->OnBeginPlay();

		sceneHierarchyPanel.SetSceneContext(activeScene);
	}

	void EditorLayer::OnSceneEndPlay()
	{
		if (activeScene->GetState() == SceneState::PLAY)
			activeScene->OnEndPlay();

		activeScene->SetState(SceneState::EDIT);

		//activeScene = editorScene;
		activeScene = Scene::Copy(editorScene);

		sceneHierarchyPanel.SetSceneContext(activeScene);
	}

	void EditorLayer::OnScenePausePlay() const
	{
		if (activeScene->GetState() == SceneState::EDIT)
			return;

		activeScene->SetState(SceneState::PAUSED);
	}
}