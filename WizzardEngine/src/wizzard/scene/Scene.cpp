//@BridgetACasey

#include "wzpch.h"

#include "Scene.h"

#include "Entity.h"
#include "wizzard/audio/Audio.h"
#include "wizzard/audio/AudioSource.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "wizzard/rendering/Renderer2D.h"
#include "component/CameraComponent.h"
#include "component/RigidBody2DComponent.h"
#include "component/SpriteComponent.h"
#include "component/UUIDComponent.h"
#include "component/TagComponent.h"
#include "component/TransformComponent.h"
#include "component/BoxCollider2DComponent.h"
#include "component/CharacterControllerComponent.h"
#include "component/AudioListenerComponent.h"
#include "wizzard/editor/ui/screenreading/ScreenReaderLogger.h"
#include "wizzard/input/Input.h"

namespace Wizzard
{
	static b2BodyType RigidBody2DTypeToBox2DBody(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::Static:    return b2_staticBody;
			case RigidBody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		WIZ_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}


	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		OnEndPlay();
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<UUIDComponent>(e).uuid;
			WIZ_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity dstEnttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	void Scene::OnBeginPlay()
	{
		sceneState = SceneState::PLAY;

		OnBeginPhysics2D();
	}

	void Scene::OnUpdatePlay(TimeStep timeStep)
	{
		//if (stepFrames-- > 0)
		//{
			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				physicsWorld->Step(timeStep, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = registry.view<RigidBody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();

					//TEMP - Player controls and related gameplay - TODO: Move this to a more appropriate location
					if (entity.HasComponent<CharacterControllerComponent>())
					{
						if(position.y < -100.0f)
						{
							ScreenReaderLogger::QueueOutput("Resetting player position");
							body->SetTransform(b2Vec2(0.0f, 2.0f), 0.0f);
							transform.Translation.x = position.x;
							transform.Translation.y = position.y;
							transform.Rotation.z = body->GetAngle();
						}

						auto& ccc = entity.GetComponent<CharacterControllerComponent>();
						body->SetFixedRotation(true);

						if (Input::IsKeyDown(Key::D))
							//transform.Translation.x += 500.0f * timeStep;
							body->SetLinearVelocity(b2Vec2(150.0f * timeStep, body->GetLinearVelocity().y));
						if (Input::IsKeyDown(Key::A))
							//transform.Translation.x -= 500.0f * timeStep;
							body->SetLinearVelocity(b2Vec2(-150.0f * timeStep, body->GetLinearVelocity().y));

						if (!ccc.disableGravity)
						{
							static float jumpForce = 2000.0f;
							body->SetGravityScale(1.0f);

							if (ccc.canJump)
							{
								if (Input::IsKeyDown(Key::Space))
								{
									body->ApplyForceToCenter(b2Vec2(0.0f, jumpForce * timeStep), true);
									jumpForce = (jumpForce < 0.0f) ? 0.0f : jumpForce - (600.0f * timeStep);

									if(jumpForce <= 0.1f)
									ccc.canJump = false;
								}
							}

							if (body->GetLinearVelocity().y > -0.001f && body->GetLinearVelocity().y < 0.001f)
							{
								ccc.canJump = true;
								jumpForce = 2000.0f;
							}
						}
						else
							body->SetGravityScale(0.0f);

						static b2ContactEdge* lastContact = nullptr;

						//Collisions with other objects
						auto contacts =  body->GetContactList();
						if(contacts)
						{
							b2AABB firstBox = body->GetFixtureList()->GetAABB(0);
							b2AABB secondBox = contacts->other->GetFixtureList()->GetAABB(0);

							//if(firstBox.lowerBound.y >= secondBox.lowerBound.y && firstBox.lowerBound.y <= secondBox.upperBound.y)
							//	WIZ_INFO("Bottom contact!!!");

							float leftDist = b2Distance(b2Vec2(firstBox.lowerBound.x, 0.0f), b2Vec2(secondBox.lowerBound.x, 0.0f));
							float rightDist = b2Distance(b2Vec2(firstBox.upperBound.x, 0.0f), b2Vec2(secondBox.upperBound.x, 0.0f));

							auto sfx = Audio::GetEditorAudioSource(WIZ_AUDIO_ENTITYMOVED);

							if (leftDist < 0.5f)
							{
								WIZ_INFO("Close on the LEFT!!!");
								if(lastContact != contacts)
								{
									sfx.SetPosition(-1.0f, 0.0f, 0.0f);
									Audio::Play(sfx);
									lastContact = contacts;
								}
							}

							else if (rightDist < 0.5f)
							{
								WIZ_INFO("Close on the RIGHT!!!");
								if (lastContact != contacts)
								{
									sfx.SetPosition(1.0f, 0.0f, 0.0f);
									Audio::Play(sfx);
									lastContact = contacts;
								}
							}
							else
							{
								lastContact = nullptr;
							}
						}
						else
						{
							lastContact = nullptr;
						}
					}
				}
			}
		//}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = registry.view<TransformComponent, CameraComponent>();

			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = registry.group<TransformComponent>(entt::get<SpriteComponent>);

			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnEndPlay()
	{
		sceneState = SceneState::EDIT;

		OnEndPhysics2D();
	}

	void Scene::OnUpdateEditor(TimeStep timeStep, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
		}

		Renderer2D::EndScene();
	}

	Entity Scene::CreateEntity(const std::string& name, bool useBaseTag)
	{
		return CreateEntityWithUUID(UUID(), name, useBaseTag);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name, bool useBaseTag)
	{
		Entity entity = { registry.create(), this };

		entity.AddComponent<UUIDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tagComponent = entity.AddComponent<TagComponent>();
		tagComponent.baseTag = name.empty() ? "Entity" : name;
		tagComponent.tag = useBaseTag ? tagComponent.baseTag : tagComponent.baseTag + std::to_string(registry.size());

		return entity;
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetBaseName().empty() ? entity.GetName() : entity.GetBaseName();
		Entity newEntity = CreateEntity(name, false);

		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<RigidBody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<CharacterControllerComponent>(newEntity, entity);
		CopyComponentIfExists<AudioListenerComponent>(newEntity, entity);

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	WizRef<Scene> Scene::CopyContentsTo(WizRef<Scene> other)
	{
		WizRef<Scene> newScene = WizRef<Scene>::CreateRef();

		newScene->viewportWidth = other->viewportWidth;
		newScene->viewportHeight = other->viewportHeight;

		auto& srcSceneRegistry = other->registry;
		auto& dstSceneRegistry = newScene->registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<UUIDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<UUIDComponent>(e).uuid;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name, true);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<RigidBody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CharacterControllerComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<AudioListenerComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		viewportWidth = width;
		viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)

				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnBeginPhysics2D()
	{
		physicsWorld = new b2World({ 0.0f, -9.8f });
		contactListener = new b2ContactListener();
		physicsWorld->SetContactListener(contactListener);
		auto view = registry.view<RigidBody2DComponent>();

		for (auto currentEntity : view)
		{
			Entity entity = { currentEntity, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			if(entity.HasComponent<CharacterControllerComponent>())
			{
				auto& cc = entity.GetComponent<CharacterControllerComponent>();

				if(cc.disableGravity)
				rb2d.Type = RigidBody2DComponent::BodyType::Static;
			}

			b2BodyDef bodyDef;
			bodyDef.type = RigidBody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnEndPhysics2D()
	{
		delete contactListener;
		contactListener = nullptr;
		delete physicsWorld;
		physicsWorld = nullptr;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<UUIDComponent>(Entity entity, UUIDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (viewportWidth > 0 && viewportHeight > 0)
			component.Camera.SetViewportSize(viewportWidth, viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CharacterControllerComponent>(Entity entity, CharacterControllerComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<AudioListenerComponent>(Entity entity, AudioListenerComponent& component)
	{
	}
}