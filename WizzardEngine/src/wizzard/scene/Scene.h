//@BridgetACasey

#pragma once

#include "entt/entt.hpp"

#include "wizzard/base/Timestep.h"
#include "wizzard/common/UUID.h"
#include "wizzard/editor/EditorCamera.h"

class b2World;

namespace Wizzard
{
	class Entity;

	enum class SceneState
	{
		EDIT = 0,
		PLAY,
		PAUSED
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnBeginPlay();
		void OnUpdatePlay(TimeStep timeStep);
		void OnEndPlay();

		void OnUpdateEditor(TimeStep timeStep, EditorCamera& camera);

		void Step(int step = 1) { stepFrames = step; }

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DuplicateEntity(Entity entity);
		void DestroyEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void SetState(const SceneState& state) { sceneState = state; }
		SceneState GetState() const { return sceneState; }

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnBeginPhysics2D();
		void OnEndPhysics2D();

		entt::registry registry;

		SceneState sceneState = SceneState::EDIT;

		uint32_t viewportWidth = 0, viewportHeight = 0;
		int stepFrames = 0;

		b2World* physicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerialiser;
		friend class SceneHierarchyPanel;
	};
}