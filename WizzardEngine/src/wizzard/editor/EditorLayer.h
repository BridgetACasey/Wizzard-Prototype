//@BridgetACasey

#pragma once

#include "wizzard/event/UIEvent.h"
#include "wizzard/event/KeyEvent.h"
#include "wizzard/event/SceneEvent.h"
#include "wizzard/event/AudioEvent.h"
#include "wizzard/audio/AudioSource.h"

#include "wizzard/common/Layer.h"
#include "wizzard/common/OrthographicCameraController.h"

#include "wizzard/rendering/Framebuffer.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/rendering/VertexArray.h"

#include "EditorCamera.h"
#include "ui/panel/ApplicationSettingsPanel.h"
#include "ui/panel/SceneHierarchyPanel.h"
#include "ui/panel/PropertiesPanel.h"

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
		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);

		void OnOverlayRender() const;
		void OnViewportToolbarRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void OnSceneBeginPlay();
		void OnSceneEndPlay();
		void OnScenePausePlay() const;

		OrthographicCameraController orthoCamController;
		EditorCamera editorCamera;

		Ref<Scene> activeScene;
		Ref<Scene> editorScene;

		Entity playerEntity;

		int gizmoType = -1;

		bool isViewportFocused = false;
		bool isViewportHovered = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		Ref<Framebuffer> frameBuffer;

		AudioSource levelMusic;
		AudioSource editorLaunchSFX;
		AudioSource errorSFX;

		ApplicationSettingsPanel appSettingsPanel;
		SceneHierarchyPanel sceneHierarchyPanel;
		//PropertiesPanel propertiesPanel;

		Entity hoveredEntity;
	};
}
