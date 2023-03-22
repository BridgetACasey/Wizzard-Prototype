//@BridgetACasey

#pragma once

#include "wizzard/event/UIEvent.h"
#include "wizzard/event/KeyEvent.h"
#include "wizzard/event/EditorEvent.h"
#include "wizzard/event/AudioEvent.h"
#include "wizzard/audio/AudioSource.h"

#include "wizzard/common/Layer.h"
#include "wizzard/common/OrthographicCameraController.h"

#include "wizzard/rendering/Framebuffer.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/rendering/VertexArray.h"

#include "EditorCamera.h"

#include "ui/panel/PanelManager.h"
#include "ui/panel/ApplicationSettingsPanel.h"
#include "ui/panel/SceneHierarchyPanel.h"
#include "ui/panel/PropertiesPanel.h"
#include "ui/panel/ViewportPanel.h"

namespace Wizzard
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(TimeStep timeStep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& keyEvent);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& mouseEvent);
		bool OnUIWindowFocus(UIWindowFocusEvent& uiEvent);
		bool OnViewportSelectionHovered(ViewportSelectionHoveredEvent& sceneEvent);
		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);
		bool OnAudioTrackStarted(AudioTrackStartedEvent& audioEvent);
		bool OnAudioTrackEnded(AudioTrackEndedEvent& audioEvent);

		void OnOverlayRender();
		void OnViewportToolbarRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void OnSceneBeginPlay();
		void OnSceneEndPlay();
		void OnScenePausePlay();

		OrthographicCameraController orthoCamController;
		EditorCamera editorCamera;

		WizScope<PanelManager> panelManager;

		WizRef<Scene> activeScene;
		WizRef<Scene> editorScene;

		Entity playerEntity;

		int gizmoType = -1;

		bool windowFocusUpdated = false;
		int focusedWindow = 0;

		bool lockSelectionToCamera = false;

		bool isViewportFocused = false;
		bool isViewportHovered = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		WizRef<Framebuffer> frameBuffer;

		AudioSource levelMusic;
		AudioSource editorLaunchSFX;
		AudioSource selectSFX;
		AudioSource errorSFX;

		WizRef<ApplicationSettingsPanel> appSettingsPanel;
		WizRef<SceneHierarchyPanel> sceneHierarchyPanel;
		WizRef<PropertiesPanel> propertiesPanel;
		WizRef<ViewportPanel> viewportPanel;

		Entity hoveredEntity;
	};
}