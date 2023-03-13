//@BridgetACasey

#pragma once

#include "wizzard/audio/AudioSource.h"

#include "wizzard/common/Layer.h"
#include "wizzard/common/OrthographicCameraController.h"

#include "wizzard/rendering/Framebuffer.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/rendering/Texture.h"
#include "wizzard/rendering/VertexArray.h"
#include "wizzard/rendering/EditorCamera.h"

#include "wizzard/editor/ui/panel/ApplicationSettingsPanel.h"
#include "wizzard/editor/ui/panel/SceneHierarchyPanel.h"
#include "wizzard/editor/ui/panel/ObjectCreationPanel.h"
#include "wizzard/editor/ui/panel/ObjectPropertiesPanel.h"

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
		void OnOverlayRender() const;

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();
		void SaveProjectAs();

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

		Entity squareEntity;
		Entity cameraEntity;

		int gizmoType = -1;

		bool primaryCamera = true;

		bool isViewportFocused = false;
		bool isViewportHovered = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec2 viewportBounds[2];

		// Temp
		Ref<VertexArray> squareVA;
		Ref<Shader> flatShader;
		Ref<Framebuffer> frameBuffer;

		Ref<Texture2D> customTexture;

		AudioSource music;
		bool playMusic = false;

		glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		ApplicationSettingsPanel appSettingsPanel;
		SceneHierarchyPanel sceneHierarchyPanel;
		ObjectCreationPanel objCreatePanel;
		ObjectPropertiesPanel objPropertiesPanel;

		Entity hoveredEntity;
	};
}