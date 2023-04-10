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
#include "editor/ui/screenreading/ScreenReaderLogger.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"
#include "wizzard/audio/Audio.h"

#include "wizzard/base/ResourcePathFinder.h"
#include "wizzard/base/Maths.h"
#include "wizzard/scene/SceneManager.h"
#include "wizzard/scene/SceneSerialiser.h"
#include <scene/component/TransformComponent.h>

#include "audio/AudioEventListener.h"
#include "scene/component/BoxCollider2DComponent.h"
#include "scene/component/CharacterControllerComponent.h"
#include "scene/component/RigidBody2DComponent.h"
#include "scene/component/TagComponent.h"
#include "EntitySelection.h"

namespace Wizzard
{
	static int WINDOW_WIDTH = 0;
	static int WINDOW_HEIGHT = 0;

	EditorLayer::EditorLayer() : Layer("Editor"), orthoCamController(0.0f, 0.0f)
	{
		WINDOW_WIDTH = Application::Get().GetWindow().GetWidth();
		WINDOW_HEIGHT = Application::Get().GetWindow().GetHeight();

		orthoCamController = OrthographicCameraController((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
	}

	void EditorLayer::OnAttach()
	{
		levelMusic = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(MUSIC, "game-comedy-interesting-playful-sweet-bright-childish-music-57040.mp3"), false);
		levelMusic.SetGain(0.25f);

		editorLaunchSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "start-computeraif-14572.mp3"), false, true, WIZ_AUDIO_EDITORSTARTUP);
		selectSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "mixkit-cool-interface-click-tone-2568.mp3"));
		errorSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "invalid-selection-39351.mp3"));
		moveEntitySFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "sound97.mp3"), true, true, WIZ_AUDIO_ENTITYMOVED);

		FramebufferSpecification fbSpec;
		fbSpec.width = WINDOW_WIDTH;
		fbSpec.height = WINDOW_HEIGHT;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		frameBuffer = Framebuffer::Create(fbSpec);

		editorScene = WizRef<Scene>::CreateRef();
		SceneManager::SetActiveScene(editorScene);

		panelManager = CreateScope<PanelManager>();

		appSettingsPanel = panelManager->AddPanel<ApplicationSettingsPanel>( PANELID_APP_SETTINGS, "PROJECT");
		sceneHierarchyPanel = panelManager->AddPanel<SceneHierarchyPanel>(PANELID_SCENE_HIERARCHY, "SCENE");
		propertiesPanel = panelManager->AddPanel<PropertiesPanel>(PANELID_PROPERTIES, "PROPERTIES");
		viewportPanel = panelManager->AddPanel<ViewportPanel>(PANELID_VIEWPORT, "VIEWPORT");
		toolbarPanel = panelManager->AddPanel<ViewportToolbarPanel>(PANELID_VIEWPORTTOOLBAR, "TOOLBAR");

		panelManager->SetSceneContext(SceneManager::GetActiveScene());

		editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		viewportPanel->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);

		Audio::QueuePlay(editorLaunchSFX);

		std::string title = "Game Editor - Example Scene";
		Application::Get().GetWindow().SetWindowTitle(title);	//TODO: Fix this to match actual scene name!
		ScreenReaderLogger::QueueOutput(title, true, true);
	}

	void EditorLayer::OnDetach()
	{
		levelMusic.FreeSource();
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		Audio::GetEventListener()->OnUpdate();
		ScreenReaderLogger::OnUpdate();

		viewportSize = viewportPanel->GetViewportSize();
		viewportBounds[0] = viewportPanel->GetViewportBounds()[0];
		viewportBounds[1] = viewportPanel->GetViewportBounds()[1];

		SceneManager::GetActiveScene()->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

		// Resize
		if (FramebufferSpecification spec = frameBuffer->GetSpecification();
			viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != viewportSize.x || spec.height != viewportSize.y))
		{
			frameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			orthoCamController.OnResize(viewportSize.x, viewportSize.y);
			editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
		}

		// Render
		Renderer2D::ResetStatistics();
		frameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		frameBuffer->ClearAttachment(1, -1);

		switch (SceneManager::GetActiveScene()->GetState())
		{
			case SceneState::EDIT:
			{
				if (viewportPanel->IsFocused())
					orthoCamController.OnUpdate(timeStep);

				editorCamera.OnUpdate(timeStep);

				panelManager->OnUpdate(timeStep);

				SceneManager::GetActiveScene()->OnUpdateEditor(timeStep, editorCamera);
				break;
			}
			case SceneState::PLAY:
			{
				SceneManager::GetActiveScene()->OnUpdatePlay(timeStep);
				break;
			}
			default:
				WIZ_ERROR("Scene State not yet implemented!");
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		glm::vec2 viewSize = viewportBounds[1] - viewportBounds[0];
		my = viewSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewSize.x && mouseY < (int)viewSize.y)
		{
			int pixelData = frameBuffer->ReadPixel(1, mouseX, mouseY);

			Entity nextHoveredEntity = pixelData == -1 ? Entity() : Entity(static_cast<entt::entity>(pixelData), SceneManager::GetActiveScene().Get());

			if(nextHoveredEntity && hoveredEntity != nextHoveredEntity)
			{
				ViewportSelectionHoveredEvent viewportEvent(nextHoveredEntity, nextHoveredEntity.GetUUID(), false);
				OnViewportSelectionHovered(viewportEvent);
			}
			hoveredEntity = nextHoveredEntity;

			//if (hoveredEntity)
			//	WIZ_TRACE("Hovered Entity: {0}", hoveredEntity.GetName());
		}

		if (!ImGuizmo::IsUsing())
		{
			if (Input::IsKeyPressed(Key::W))
			{
				viewportPanel->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
				ScreenReaderLogger::QueueOutput("Transform type: Translate");
			}
			if (Input::IsKeyPressed(Key::E))
			{
				viewportPanel->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
				ScreenReaderLogger::QueueOutput("Transform type: Rotate");
			}
			if (Input::IsKeyPressed(Key::R))
			{
				viewportPanel->SetGizmoType(ImGuizmo::OPERATION::SCALE);
				ScreenReaderLogger::QueueOutput("Transform type: Scale");
			}
		}

		if (SceneManager::GetActiveScene()->GetState() == SceneState::EDIT)
		{
			if (Input::IsKeyPressed(Key::Space) || Input::IsMouseButtonPressed(Mouse::LeftButton))
			{
				if (viewportPanel->IsFocused() && hoveredEntity)
				{
					//sceneHierarchyPanel.SetSelectedEntity(hoveredEntity);

					ViewportSelectionChangedEvent sceneEvent(hoveredEntity, hoveredEntity.GetUUID(), true);
					OnViewportSelectionChanged(sceneEvent);
					//editorCamera.FocusOnPoint(hoveredEntity.GetComponent<TransformComponent>().Translation);
				}
			}
		}
		//if (Input::IsKeyPressed(Key::Z))
		if (Input::IsActionTriggered(WIZ_IA_RETRYDETECTSCREENREADER))
		{
			WIZ_TRACE("Attempting to detect screen reader at runtime.");
			ScreenReaderLogger::DetectScreenReader();
		}

		if(Input::IsActionTriggered(WIZ_IA_TOGGLECAMENTITYLOCK))
		{
			lockSelectionToCamera = !lockSelectionToCamera;
		
			if (lockSelectionToCamera)
				ScreenReaderLogger::QueueOutput("Enabled camera lock on selections");
			else
				ScreenReaderLogger::QueueOutput("Disabled camera lock on selections");
		}

		//if (Input::IsActionTriggered(WIZ_IA_CYCLEUIWINDOW))
		//	panelManager->CycleActivePanel();

		OnOverlayRender();

		frameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		WIZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;//ImGuiDockNodeFlags_AutoHideTabBar;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;// ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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

		panelManager->OnImGuiRender();

		ImGui::End();	//End Dockspace Demo
	}

	void EditorLayer::OnEvent(Event& event)
	{
		Audio::GetEventListener()->OnEvent(event);
		ScreenReaderLogger::OnEvent(event);

		orthoCamController.OnEvent(event);

		if(SceneManager::GetActiveScene()->GetState() == SceneState::EDIT)
		editorCamera.OnEvent(event);

		panelManager->OnEvent(event);

		EventHandler eventHandler(event);
		eventHandler.HandleEvent<KeyPressedEvent>(WIZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		eventHandler.HandleEvent<MouseButtonPressedEvent>(WIZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		eventHandler.HandleEvent<ViewportSelectionHoveredEvent>(WIZ_BIND_EVENT_FN(EditorLayer::OnViewportSelectionHovered));
		eventHandler.HandleEvent<ViewportSelectionChangedEvent>(WIZ_BIND_EVENT_FN(EditorLayer::OnViewportSelectionChanged));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& keyEvent)
	{
		if (Input::IsQueryingInput())
		{
			if (keyEvent.GetKeyCode() == Key::F)
			{
				Entity selectedEntity = propertiesPanel->GetSelectedEntity();
				if (selectedEntity)
					editorCamera.FocusOnPoint(selectedEntity.GetComponent<TransformComponent>().Translation);
			}

			if (keyEvent.GetKeyCode() == Key::Escape)
			{
				Entity selectedEntity = propertiesPanel->GetSelectedEntity();

				if (selectedEntity)
				{
					propertiesPanel->SetSelectedEntity({});

					if (EntitySelection::IsMultiSelect())
					{
						EntitySelection::DeselectAll();
						ScreenReaderLogger::QueueOutput("Deselected all");
					}
					else
					{
						EntitySelection::DeselectEntity(selectedEntity);
						glm::vec3 trans = selectedEntity.GetComponent<TransformComponent>().Translation;
						ScreenReaderLogger::QueueOutput("Deselected " + selectedEntity.GetName() + " at X " + std::to_string((int)trans.x) + " Y " + std::to_string((int)trans.y));
					}
				}
			}

			if (keyEvent.GetKeyCode() == Key::S)
			{
				if (propertiesPanel->GetSelectedEntity())
				{
					ScreenReaderLogger::QueueOutput("Deleted " + propertiesPanel->GetSelectedEntity().GetName());
					SceneManager::GetActiveScene()->DestroyEntity(propertiesPanel->GetSelectedEntity());
				}
			}

			if (keyEvent.GetKeyCode() == Key::D)
			{
				if (propertiesPanel->GetSelectedEntity())
				{
					ScreenReaderLogger::QueueOutput("Duplicated " + propertiesPanel->GetSelectedEntity().GetBaseName());
					Entity duplicatedEntity = SceneManager::GetActiveScene()->DuplicateEntity(propertiesPanel->GetSelectedEntity());

					if (!EntitySelection::IsMultiSelect())
						EntitySelection::DeselectAll();

					EntitySelection::SelectEntity(duplicatedEntity);
					propertiesPanel->SetSelectedEntity(duplicatedEntity);
				}
			}

			if (keyEvent.GetKeyCode() == Key::M)
			{
				EntitySelection::ToggleMultiSelectMode();

				if (EntitySelection::IsMultiSelect())
					ScreenReaderLogger::QueueOutput("Enabled entity multiselect mode");
				else
				{
					EntitySelection::DeselectAll();
					ScreenReaderLogger::QueueOutput("Disabled entity multiselect mode");
				}
			}

			if (keyEvent.GetKeyCode() == Key::O)
			{
				EntitySelection::DescribeSelectionsIsolated();
			}

			if (keyEvent.GetKeyCode() == Key::P)
			{
				EntitySelection::DescribeSelectionsRelational();
			}

			if (keyEvent.GetKeyCode() == Key::LeftShift)
			{
				panelManager->CycleActivePanel();
			}

			if (keyEvent.GetKeyCode() == Key::Space)
			{
				if (viewportPanel->IsHovered() /* && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)*/)
				{
					EntitySelection::SelectEntity(hoveredEntity);
					propertiesPanel->SetSelectedEntity(hoveredEntity);
				}
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& mouseEvent)
	{
		if (Input::IsQueryingInput())
		{
			if (mouseEvent.GetMouseButton() == Mouse::LeftButton)
			{
				if (viewportPanel->IsHovered() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				{
					if (!EntitySelection::IsMultiSelect())
						EntitySelection::DeselectAll();

					propertiesPanel->SetSelectedEntity(hoveredEntity);
					EntitySelection::SelectEntity(hoveredEntity);
				}
			}
		}

		return false;
	}

	bool EditorLayer::OnViewportSelectionHovered(ViewportSelectionHoveredEvent& sceneEvent)
	{
		ScreenReaderLogger::QueueOutput(sceneEvent.GetSelectionContext().GetName());

		return false;
	}

	bool EditorLayer::OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent)
	{
		glm::vec3 trans = sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Translation;
		editorCamera.FocusOnPoint(trans);

		//if(hoveredEntity != sceneEvent.GetSelectionContext())
		ScreenReaderLogger::QueueOutput(sceneEvent.GetSelectionContext().GetName() + " at X " + std::to_string((int)trans.x) + ", Y " + std::to_string((int)trans.y), true, true);

		viewportPanel->SetEntityBaseTranslation(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Translation);
		viewportPanel->SetEntityBaseRotation(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Rotation);
		viewportPanel->SetEntityBaseScale(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Scale);
		viewportPanel->SetMoveUnitCount(0);
		Audio::GetEditorAudioSource(WIZ_AUDIO_ENTITYMOVED).SetPitch(1.0f);

		Audio::Play(selectSFX);

		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (SceneManager::GetActiveScene()->GetState() == SceneState::PLAY)
		{
			Entity camera = SceneManager::GetActiveScene()->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
			Renderer2D::BeginScene(editorCamera);

		Renderer2D::EndScene();
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
		SceneManager::GetActiveScene()->SetState(SceneState::PLAY);

		SceneManager::SetActiveScene(Scene::CopyContentsTo(editorScene));
		SceneManager::GetActiveScene()->OnBeginPlay();

		panelManager->SetSceneContext(SceneManager::GetActiveScene());
		EntitySelection::DeselectAll();
		propertiesPanel->SetSelectedEntity({});

		viewportPanel->SetShouldTriggerFocus(true);

		Audio::Play(levelMusic);
	}

	void EditorLayer::OnSceneEndPlay()
	{
		if (SceneManager::GetActiveScene()->GetState() == SceneState::PLAY)
			SceneManager::GetActiveScene()->OnEndPlay();

		SceneManager::SetActiveScene(editorScene);

		SceneManager::GetActiveScene()->SetState(SceneState::EDIT);
		panelManager->SetSceneContext(SceneManager::GetActiveScene());

		Audio::Stop(levelMusic);
	}
}