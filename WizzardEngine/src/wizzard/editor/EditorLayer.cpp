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
		errorSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "invalid-selection-39351.mp3"), false, true, WIZ_AUDIO_EDITORSHUTDOWN);
		moveEntitySFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "sound97.mp3"), true, true, WIZ_AUDIO_ENTITYMOVED);

		//TEMP AUDIO FILES
		playerJumpSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "retro-game-sfx_jump-bumpwav-14853.mp3"), false, true, WIZ_AUDIO_PLAYERJUMP);
		playerLandSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "FEETHmn-Jumping-on-concrete-2-(ID-1836)-BSB.mp3"), false, true, WIZ_AUDIO_PLAYERLAND);

		FramebufferSpecification fbSpec;
		fbSpec.width = WINDOW_WIDTH;
		fbSpec.height = WINDOW_HEIGHT;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		frameBuffer = Framebuffer::Create(fbSpec);

		editorScene = WizRef<Scene>::CreateRef();
		SceneManager::SetActiveScene(editorScene);

		InitTutorialMessages();

		panelManager = CreateScope<PanelManager>();

		sceneHierarchyPanel = panelManager->AddPanel<SceneHierarchyPanel>(PANELID_SCENE_HIERARCHY, "SCENE");
		propertiesPanel = panelManager->AddPanel<PropertiesPanel>(PANELID_PROPERTIES, "PROPERTIES");
		viewportPanel = panelManager->AddPanel<ViewportPanel>(PANELID_VIEWPORT, "VIEWPORT");
		toolbarPanel = panelManager->AddPanel<ViewportToolbarPanel>(PANELID_VIEWPORTTOOLBAR, "TOOLBAR");
		appSettingsPanel = panelManager->AddPanel<ApplicationSettingsPanel>(PANELID_APP_SETTINGS, "PROJECT");

		panelManager->SetSceneContext(SceneManager::GetActiveScene());

		editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		viewportPanel->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);

		Audio::QueuePlay(editorLaunchSFX);

		Input::SetMousePosition(0.0f, 0.0f);

		std::string title = "WIZZARD Game Editor - Untitled Scene";
		Application::Get().GetWindow().SetWindowTitle(title);	//TODO: Fix this to match actual scene name!
		//ScreenReaderLogger::QueueOutput(title, false, true);

		ScreenReaderLogger::QueueOutput(tutorialMessages.at("EditorLaunch").first, false, true);
		tutorialMessages.at("EditorLaunch").second = true;
		IncrementTutorialMessagesPlayed();
	}

	void EditorLayer::OnDetach()
	{
		levelMusic.FreeSource();
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		fps = 1.0f / timeStep;

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

		panelManager->OnUpdate(timeStep);

		switch (SceneManager::GetActiveScene()->GetState())
		{
			case SceneState::EDIT:
			{
				if (viewportPanel->IsFocused())
					orthoCamController.OnUpdate(timeStep);

				editorCamera.OnUpdate(timeStep);

				SceneManager::GetActiveScene()->OnUpdateEditor(timeStep, editorCamera);
				break;
			}
			case SceneState::PLAY:
			{
				//if (Input::IsKeyPressed(Key::Space))
				//	SceneManager::GetActiveScene()->playerJumpSpeed = 5000.0f * timeStep;
				//else
				//	SceneManager::GetActiveScene()->playerJumpSpeed = 0.0f;
				//
				//if (Input::IsKeyDown(Key::D))
				//	SceneManager::GetActiveScene()->playerMoveSpeed = 500.0f * timeStep;
				//else if(Input::IsKeyDown(Key::A))
				//	SceneManager::GetActiveScene()->playerMoveSpeed = -500.0f * timeStep;
				//else
				//	SceneManager::GetActiveScene()->playerMoveSpeed = 0.0f;

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

		if (SceneManager::GetActiveScene()->GetState() == SceneState::EDIT)
		{
			if (!ImGuizmo::IsUsing())
			{
				if (Input::IsKeyPressed(Key::W))
				{
					if(!EntitySelection::GetSelections().empty())
					{
						viewportPanel->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);

						if (enableTutorialMessages)
							TriggerTutorialMessage("TransformTranslate", true);
						else
							ScreenReaderLogger::QueueOutput("Transform type: Translate");
					}
				}
				if (Input::IsKeyPressed(Key::E))
				{
					if (!EntitySelection::GetSelections().empty())
					{
						viewportPanel->SetGizmoType(ImGuizmo::OPERATION::ROTATE);

						if (enableTutorialMessages)
							TriggerTutorialMessage("TransformRotate", true);
						else
							ScreenReaderLogger::QueueOutput("Transform type: Rotate");
					}
				}
				if (Input::IsKeyPressed(Key::R))
				{
					if (!EntitySelection::GetSelections().empty())
					{
						viewportPanel->SetGizmoType(ImGuizmo::OPERATION::SCALE);

						if (enableTutorialMessages)
							TriggerTutorialMessage("TransformScale", true);
						else
							ScreenReaderLogger::QueueOutput("Transform type: Scale");
					}
				}
			}

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

	bool EditorLayer::TriggerTutorialMessage(const std::string& name, bool shouldInterrupt)
	{
		if (enableTutorialMessages)
		{
			if (!tutorialMessages.at(name).second)
			{
				ScreenReaderLogger::QueueOutput(tutorialMessages.at(name).first, shouldInterrupt, true);
				tutorialMessages.at(name).second = true;
				IncrementTutorialMessagesPlayed();

				return true;
			}
		}

		return false;
	}

	void EditorLayer::SetEnableTutorialMessages(bool enable)
	{
		enableTutorialMessages = enable;

		if(enableTutorialMessages)
		{
			tutorialMessagesPlayed = 0;

			for(auto message : tutorialMessages)
				message.second.second = false;
		}
	}

	void EditorLayer::IncrementTutorialMessagesPlayed()
	{
		tutorialMessagesPlayed++;

		if(tutorialMessagesPlayed >= tutorialMessages.size())
		{
			enableTutorialMessages = false;
		}
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

					if (!EntitySelection::IsMultiSelect() || EntitySelection::GetSelections().size() == 1)
					{
						EntitySelection::DeselectEntity(selectedEntity);
						glm::vec3 trans = selectedEntity.GetComponent<TransformComponent>().Translation;

						ScreenReaderLogger::QueueOutput("Deselected " + selectedEntity.GetName() + " at X " + std::to_string((int)trans.x) + " Y " + std::to_string((int)trans.y));

						TriggerTutorialMessage("ViewportEntityDeselected", true);
					}
					else
					{
						EntitySelection::DeselectAll();
						ScreenReaderLogger::QueueOutput("Deselected all");
					}
				}
			}

			if(SceneManager::GetActiveScene()->GetState() == SceneState::EDIT)
			{
				if (keyEvent.GetKeyCode() == Key::S)
				{
					if (propertiesPanel->GetSelectedEntity())
					{
						ScreenReaderLogger::QueueOutput("Deleted " + propertiesPanel->GetSelectedEntity().GetName(), false, true);
						EntitySelection::DeselectAll();
						SceneManager::GetActiveScene()->DestroyEntity(propertiesPanel->GetSelectedEntity());
						propertiesPanel->SetSelectedEntity({});
					}
				}

				if (keyEvent.GetKeyCode() == Key::D)
				{
					if (propertiesPanel->GetSelectedEntity())
					{
						ScreenReaderLogger::QueueOutput("Duplicated " + propertiesPanel->GetSelectedEntity().GetBaseName(), false, true);
						Entity duplicatedEntity = SceneManager::GetActiveScene()->DuplicateEntity(propertiesPanel->GetSelectedEntity());

						if (!EntitySelection::IsMultiSelect())
							EntitySelection::DeselectAll();

						EntitySelection::SelectEntity(duplicatedEntity);
						propertiesPanel->SetSelectedEntity(duplicatedEntity);
					}
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

			if (keyEvent.GetKeyCode() == Key::LeftControl || keyEvent.GetKeyCode() == Key::RightControl)
			{
				panelManager->CycleActivePanel();
			}

			if (keyEvent.GetKeyCode() == Key::Space)
			{
				if (viewportPanel->IsHovered() /* && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)*/)
				{
					EntitySelection::SelectEntity(hoveredEntity);
					propertiesPanel->SetSelectedEntity(hoveredEntity);

					if (EntitySelection::GetSelections().size() >= 2)
						Application::Get().GetEditorLayer()->TriggerTutorialMessage("EntityMultiSelected", true);
					else
						TriggerTutorialMessage("ViewportEntitySelected", true);
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

					//if (EntitySelection::GetSelections().size() >= 2)
					//	Application::Get().GetEditorLayer()->TriggerTutorialMessage("EntityMultiSelected");
					//else
					//	TriggerTutorialMessage("ViewportEntitySelected");
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
		ScreenReaderLogger::QueueOutput(sceneEvent.GetSelectionContext().GetName() + " at X " + std::to_string((int)trans.x) + ", Y " + std::to_string((int)trans.y), !enableTutorialMessages, true);
		Audio::GetEditorAudioSource(WIZ_AUDIO_ENTITYMOVED).SetPitch(1.0f);
		Audio::Play(selectSFX);

		viewportPanel->SetEntityBaseTranslation(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Translation);
		viewportPanel->SetEntityBaseRotation(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Rotation);
		viewportPanel->SetEntityBaseScale(sceneEvent.GetSelectionContext().GetComponent<TransformComponent>().Scale);
		viewportPanel->SetMoveUnitCount(0);

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

	void EditorLayer::InitTutorialMessages()
	{
		//Each message only triggers the first time the respective action has been performed. Once all messages have been played, tutorial messages are switched off in application settings.

		//Editor Launch
		std::string editorLaunchMessage = "Welcome to the Wizzard Game Editor. The goal of Wizzard is to allow you to manipulate objects in a scene to make a basic level for a 2D platformer game, without the use of your vision. ";
		editorLaunchMessage.append("This tool is designed to function similarly to a typical game engine editor, and it features a viewport, a 2D scene, and a hierarchy of objects you can place. ");
		editorLaunchMessage.append("The default scene has a controllable character and a static floor. Press the LEFT CONTROL key to switch between panel windows, and the TAB key to cycle through that panel's interactive elements. ");
		editorLaunchMessage.append("You can also use arrow keys for menu navigation. End Tutorial Message.");

		tutorialMessages.emplace("EditorLaunch", std::pair(editorLaunchMessage, false));


		//Viewport
		std::string viewportMessage = "VIEWPORT panel. This panel allows you to manipulate the scene's objects directly in 2D space. Use the arrow keys to move the editor camera up and down. ";
		viewportMessage.append("Your cursor is locked to the middle of the viewport screen by default. Press SPACE when hovering over an entity to select it. End Tutorial Message.");

		tutorialMessages.emplace("Viewport", std::pair(viewportMessage, false));


		//Viewport Toolbar
		std::string viewportToolbarMessage = "VIEWPORT TOOLBAR. When you are ready to simulate your scene in the editor, press the PLAY button in this toolbar. ";
		viewportToolbarMessage.append("To stop and reset your scene, navigate back to this toolbar during runtime and press the STOP button. End Tutorial Message.");

		tutorialMessages.emplace("ViewportToolbar", std::pair(viewportToolbarMessage, false));


		//Entity Selected in Viewport
		std::string viewportEntitySelectedMessage = "You selected an entity. Use the arrow keys to move it around. You will hear a directional sound effect that increases in pitch the further away you move the object from it's original position. ";
		viewportEntitySelectedMessage.append("Each ping indicates you have moved by 0.5 units. You can switch to different transform modes by pressing the W, E, and R keys. ");
		viewportEntitySelectedMessage.append("You can also duplicate your currently selection with the 'D' key, or delete it from the scene with the 'S' key. When you are happy, press ESCAPE to deselect the entity. End Tutorial Message.");

		tutorialMessages.emplace("ViewportEntitySelected", std::pair(viewportEntitySelectedMessage, false));


		//Transform Type - Translate (with an entity actively selected)
		std::string transformTranslateMessage = "Transform type: Translate selected. Use the arrow keys to move your entity around. ";
		transformTranslateMessage.append("You will hear a directional sound effect that increases in pitch the further away you move the object from it's original position. End Tutorial Message.");

		tutorialMessages.emplace("TransformTranslate", std::pair(transformTranslateMessage, false));


		//Transform Type - Rotate (with an entity actively selected)
		std::string transformRotateMessage = "Transform type: Rotate selected. Use the left and right arrow keys to rotate your entity in forward axis. Each press rotates the object by 10 degrees. End Tutorial Message.";

		tutorialMessages.emplace("TransformRotate", std::pair(transformRotateMessage, false));


		//Transform Type - Scale (with an entity actively selected)
		std::string transformScaleMessage = "Transform type: Scale selected. Use the up and down arrow keys to make your entity larger or smaller respectively. Each press scales the object by 0.1 units. End Tutorial Message.";

		tutorialMessages.emplace("TransformScale", std::pair(transformScaleMessage, false));


		//Entity Deselected in Viewport
		std::string viewportEntityDeselectedMessage = "You deselected an entity. You can also enable the ability to select and transform multiple entities at the same time by pressing the M key. ";
		viewportEntityDeselectedMessage.append("You can press the CAPSLOCK key to switch between moving the scene camera and transforming your selected entities when you press the arrow keys. This also frees your mouse cursor inside the viewport. ");
		viewportEntityDeselectedMessage.append("To add new entities, you can open the ENTITY CREATION menu by pressing the C key, or the 'CREATE ENTITY' button in the SCENE HIERARCHY panel. End Tutorial Message.");

		tutorialMessages.emplace("ViewportEntityDeselected", std::pair(viewportEntityDeselectedMessage, false));


		//Multiple Entities Selected at Once (in Scene Hierarchy or in Viewport)
		std::string entityMultiSelectedMessage = "You've selected multiple entities. If you are ever unsure about how your scene looks, you can press the O and P keys to output information about your selections to your screen reader. ";
		entityMultiSelectedMessage.append("The 'O' key prints information about individual entities and their components. ");
		entityMultiSelectedMessage.append("The 'P' key prints information about the relationships between each selection and how they interact. End Tutorial Message.");

		tutorialMessages.emplace("EntityMultiSelected", std::pair(entityMultiSelectedMessage, false));


		//Application Settings
		std::string appSettingsMessage = "PROJECT panel. Here you can save and load scenes you have worked on, or toggle some of the editor's settings. End Tutorial Message.";

		tutorialMessages.emplace("ApplicationSettings", std::pair(appSettingsMessage, false));


		//Object Properties
		std::string objPropertiesMessage = "PROPERTIES panel. If you have selected an entity from the SCENE HIERARCHY or the VIEWPORT, their components will appear here, and you can change their values. ";
		objPropertiesMessage.append("Attaching different components will apply different properties to the entity. ");
		objPropertiesMessage.append("Component types include Transform for translation, rotation, and scaling; Sprite for colour changes; Rigid Body 2D for physics; and Box Collider 2D for collisions. End Tutorial Message.");

		tutorialMessages.emplace("ObjectProperties", std::pair(objPropertiesMessage, false));


		//Scene Hierarchy
		std::string sceneHierarchyMessage = "SCENE HIERARCHY panel. This panel contains a list of all your objects currently in the scene. TAB through each object to select it. You can also add new entities from a list of presets with the 'CREATE ENTITY' button. ";
		sceneHierarchyMessage.append("To edit the components of your last selection, navigate to the PROPERTIES panel. Alternatively, to manipulate it directly in 2D space, navigate to the VIEWPORT panel. End Tutorial Message.");

		tutorialMessages.emplace("SceneHierarchy", std::pair(sceneHierarchyMessage, false));


		//Entered Play Mode
		std::string playModeEnteredMessage = "The game scene is now playing. Most editing functionality will be disabled until you choose to end the simulation. Use the A and D keys to move the player character left and right. Press SPACE to jump. ";
		playModeEnteredMessage.append("You should hear a directional sound cue when you approach the edge of the object you are standing on. There should also be cues for jumping and landing successfully on a platform. ");
		playModeEnteredMessage.append("To stop simulating the scene, navigate to the VIEWPORT TOOLBAR and press the STOP button. End Tutorial Message.");

		tutorialMessages.emplace("PlayModeEntered", std::pair(playModeEnteredMessage, false));
	}

	void EditorLayer::NewScene()
	{
		SceneManager::SetActiveScene(WizRef<Scene>::CreateRef());
		sceneHierarchyPanel->SetSceneContext(SceneManager::GetActiveScene());

		editorScenePath = std::filesystem::path();
	}

	void EditorLayer::LoadScene()
	{
		std::string filepath = SceneSerialiser::OpenFile("WIZZARD Scene (*.wizzard)\0*.wizzard\0");
		if (!filepath.empty())
			LoadScene(filepath);
	}

	void EditorLayer::LoadScene(const std::filesystem::path& path)
	{
		if (SceneManager::GetActiveScene()->GetState() != SceneState::EDIT)
			OnSceneEndPlay();

		if(path == editorScenePath)
		{
			WIZ_WARN("Scene is currently open in editor!");
			return;
		}

		if (path.extension().string() != ".wizzard")
		{
			WIZ_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		WizRef<Scene> newScene = WizRef<Scene>::CreateRef();
		SceneSerialiser serialiser(newScene);
		if (serialiser.Deserialise(path.string()))
		{
			editorScene = newScene;
			sceneHierarchyPanel->SetSceneContext(editorScene);

			SceneManager::SetActiveScene(editorScene);
			editorScenePath = path;

			std::string title = "WIZZARD Game Editor - ";
			title.append(path.filename().string());
			Application::Get().GetWindow().SetWindowTitle(title);
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!editorScenePath.empty())
		{
			SceneSerialiser serialiser(SceneManager::GetActiveScene());
			serialiser.Serialise(editorScenePath.string());
		}
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = SceneSerialiser::SaveFile("WIZZARD Scene (*.wizzard)\0*.wizzard\0");
		if (!filepath.empty())
		{
			SceneSerialiser serialiser(SceneManager::GetActiveScene());
			serialiser.Serialise(filepath);
			editorScenePath = filepath;
		}
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

		TriggerTutorialMessage("PlayModeEntered", true);
	}

	void EditorLayer::OnSceneEndPlay()
	{
		if (SceneManager::GetActiveScene()->GetState() == SceneState::PLAY)
			SceneManager::GetActiveScene()->OnEndPlay();

		SceneManager::SetActiveScene(editorScene);

		SceneManager::GetActiveScene()->SetState(SceneState::EDIT);
		panelManager->SetSceneContext(SceneManager::GetActiveScene());
		EntitySelection::DeselectAll();
		propertiesPanel->SetSelectedEntity({});

		Audio::Stop(levelMusic);
	}
}