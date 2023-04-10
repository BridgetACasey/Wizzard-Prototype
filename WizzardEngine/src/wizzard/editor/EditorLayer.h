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

		void SetEnableTutorialMessages(bool enable) { enableTutorialMessages = enable; }
		bool GetEnableTutorialMessages() { return enableTutorialMessages; }

		//Temp functions
		EditorCamera& GetEditorCamera() { return editorCamera; }
		WizRef<Framebuffer>& GetFrameBuffer() { return frameBuffer; }
		WizRef<PropertiesPanel>& GetPropertiesPanel() { return propertiesPanel; }
		Entity GetSelectedEntity() { return propertiesPanel->GetSelectedEntity(); }
		bool GetLockOnSelection() const { return lockSelectionToCamera; }

		void OnSceneBeginPlay();
		void OnSceneEndPlay();

	private:
		bool OnKeyPressed(KeyPressedEvent& keyEvent);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& mouseEvent);
		bool OnViewportSelectionHovered(ViewportSelectionHoveredEvent& sceneEvent);
		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);

		void OnOverlayRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		OrthographicCameraController orthoCamController;
		EditorCamera editorCamera;

		WizScope<PanelManager> panelManager;
		WizRef<ApplicationSettingsPanel> appSettingsPanel;
		WizRef<SceneHierarchyPanel> sceneHierarchyPanel;
		WizRef<PropertiesPanel> propertiesPanel;
		WizRef<ViewportPanel> viewportPanel;
		WizRef<ViewportToolbarPanel> toolbarPanel;

		Entity hoveredEntity;

		WizRef<Scene> editorScene;

		Entity playerEntity;

		bool lockSelectionToCamera = true;

		bool enableTutorialMessages = true;

		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		WizRef<Framebuffer> frameBuffer;

		AudioSource levelMusic;
		AudioSource editorLaunchSFX;
		AudioSource selectSFX;
		AudioSource errorSFX;
		AudioSource moveEntitySFX;
	};
}