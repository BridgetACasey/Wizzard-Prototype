//@BridgetACasey

#pragma once

#include "entt/entt.hpp"

#include "wizzard/base/Timestep.h"
#include "wizzard/common/UUID.h"
#include "wizzard/editor/EditorCamera.h"

class b2World;
class b2ContactListener;

namespace Wizzard
{
	class Entity;

	enum class SceneState
	{
		EDIT = 0,
		PLAY
		//,PAUSED	//TODO: Pause functionality to be reimplemented at a later date
	};

	class Scene : public WizRefCounter
	{
	public:
		Scene();
		~Scene();

		void OnBeginPlay();
		void OnUpdatePlay(TimeStep timeStep);
		void OnEndPlay();

		void OnUpdateEditor(TimeStep timeStep, EditorCamera& camera);

		void Step(int step = 1) { stepFrames = step; }

		static WizRef<Scene> CopyContentsTo(WizRef<Scene> other);

		Entity CreateEntity(const std::string& name = std::string(), bool useBaseTag = true);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string(), bool useBaseTag = true);
		Entity DuplicateEntity(Entity entity);
		void DestroyEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void SetState(const SceneState& state) { sceneState = state; }
		SceneState GetState() const { return sceneState; }

		float playerMoveSpeed = 0.0f;
		float playerJumpSpeed = 0.0f;

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
		b2ContactListener* contactListener = nullptr;

		friend class Entity;
		friend class SceneSerialiser;
		friend class SceneHierarchyPanel;
	};
}