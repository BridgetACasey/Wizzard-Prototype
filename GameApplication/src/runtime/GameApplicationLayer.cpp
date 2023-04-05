//@BridgetACasey

#include "GameApplicationLayer.h"

GameApplicationLayer::GameApplicationLayer() : Layer("SceneApp2D")
{
}

void GameApplicationLayer::OnAttach()
{
	gameScene = Wizzard::WizRef<Wizzard::Scene>::CreateRef();
	Wizzard::Application::Get().GetEditorLayer()->SetActiveScene(gameScene);

	// Entity - playable character, hence camera attached
	auto square = gameScene->CreateEntity("Green Square");
	square.GetComponent<Wizzard::TagComponent>().baseTag = "Green Square";
	square.AddComponent<Wizzard::SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	square.AddComponent<Wizzard::RigidBody2DComponent>();
	square.GetComponent<Wizzard::RigidBody2DComponent>().Type = Wizzard::RigidBody2DComponent::BodyType::Dynamic;
	square.AddComponent<Wizzard::BoxCollider2DComponent>();
	square.AddComponent<Wizzard::CameraComponent>();
	square.AddComponent<Wizzard::CharacterControllerComponent>();
	
	// Entity
	auto squareTwo = gameScene->CreateEntity("Red Square");
	squareTwo.GetComponent<Wizzard::TagComponent>().baseTag = "Red Square";
	squareTwo.AddComponent<Wizzard::SpriteComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
	squareTwo.GetComponent<Wizzard::TransformComponent>().Translation.x -= 2.0f;
	squareTwo.AddComponent<Wizzard::RigidBody2DComponent>();
	squareTwo.GetComponent<Wizzard::RigidBody2DComponent>().Type = Wizzard::RigidBody2DComponent::BodyType::Dynamic;
	squareTwo.AddComponent<Wizzard::BoxCollider2DComponent>();
	
	// Entity
	auto squareThree = gameScene->CreateEntity("Blue Square");
	squareThree.GetComponent<Wizzard::TagComponent>().baseTag = "Blue Square";
	squareThree.AddComponent<Wizzard::SpriteComponent>(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
	squareThree.GetComponent<Wizzard::TransformComponent>().Translation.x += 2.0f;
	squareThree.AddComponent<Wizzard::RigidBody2DComponent>();
	squareThree.GetComponent<Wizzard::RigidBody2DComponent>().Type = Wizzard::RigidBody2DComponent::BodyType::Dynamic;
	squareThree.AddComponent<Wizzard::BoxCollider2DComponent>();
	
	// Entity
	auto floor = gameScene->CreateEntity("Floor");
	floor.GetComponent<Wizzard::TagComponent>().baseTag = "Floor";
	floor.AddComponent<Wizzard::SpriteComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	floor.GetComponent<Wizzard::TransformComponent>().Translation.y -= 3.5f;
	floor.GetComponent<Wizzard::TransformComponent>().Scale.y *= 0.5f;
	floor.GetComponent<Wizzard::TransformComponent>().Scale.x *= 5.0f;
	floor.AddComponent<Wizzard::RigidBody2DComponent>();
	floor.GetComponent<Wizzard::RigidBody2DComponent>().Type = Wizzard::RigidBody2DComponent::BodyType::Static;
	floor.AddComponent<Wizzard::BoxCollider2DComponent>();
	
	playerEntity = square;
}

void GameApplicationLayer::OnDetach()
{
}

void GameApplicationLayer::OnUpdate(Wizzard::TimeStep timeStep)
{
}

void GameApplicationLayer::OnImGuiRender()
{
}

void GameApplicationLayer::OnEvent(Wizzard::Event& event)
{
}