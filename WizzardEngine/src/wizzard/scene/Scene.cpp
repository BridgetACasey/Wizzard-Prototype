//@BridgetACasey

#include "wzpch.h"

#include "Scene.h"

#include "Entity.h"
#include "wizzard/rendering/Renderer2D.h"
#include "component/CameraComponent.h"
#include "component/SpriteComponent.h"
#include "component/TagComponent.h"
#include "component/TransformComponent.h"

namespace Wizzard
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();

		auto& tagComponent = entity.AddComponent<TagComponent>();
		tagComponent.tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(TimeStep timeStep)
	{
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = registry.view<TransformComponent, CameraComponent>();

			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = registry.group<TransformComponent>(entt::get<SpriteComponent>);

			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}