//@BridgetACasey

#include "GameApplicationLayer.h"

#include "box2d/b2_body.h"
#include "box2d/b2_math.h"

GameApplicationLayer::GameApplicationLayer() : Layer("SceneApp2D")
{
}

void GameApplicationLayer::OnAttach()
{
	//gameScene = Wizzard::WizRef<Wizzard::Scene>::CreateRef();
	//Wizzard::Application::Get().GetEditorLayer()->SetActiveScene(gameScene);
	gameScene = Wizzard::SceneManager::GetActiveScene();

	// Entity - playable character, hence camera attached
	auto player = gameScene->CreateEntity("Player");
	player.GetComponent<Wizzard::TagComponent>().baseTag = "Player";
	player.AddComponent<Wizzard::SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
	player.AddComponent<Wizzard::RigidBody2DComponent>();
	player.GetComponent<Wizzard::RigidBody2DComponent>().Type = Wizzard::RigidBody2DComponent::BodyType::Dynamic;
	player.AddComponent<Wizzard::BoxCollider2DComponent>();
	player.AddComponent<Wizzard::CameraComponent>();
	player.AddComponent<Wizzard::CharacterControllerComponent>();
	
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
	
	playerEntity = player;
}

void GameApplicationLayer::OnDetach()
{
}

void GameApplicationLayer::OnUpdate(Wizzard::TimeStep timeStep)
{
	if (gameScene->GetState() == Wizzard::SceneState::PLAY)
	{
		//if (playerEntity.HasComponent<Wizzard::CharacterControllerComponent>())
		//{
		//	auto& transform = playerEntity.GetComponent<Wizzard::TransformComponent>();
		//	auto& rb2d = playerEntity.GetComponent<Wizzard::RigidBody2DComponent>();
		//	auto& ccc = playerEntity.GetComponent<Wizzard::CharacterControllerComponent>();
		//
		//	b2Body* body = (b2Body*)rb2d.RuntimeBody;
		//	body->SetFixedRotation(true);
		//
		//	if (Wizzard::Input::IsKeyDown(Wizzard::Key::D))
		//		//transform.Translation.x += 500.0f * timeStep;
		//		body->SetLinearVelocity(b2Vec2(100.0f * timeStep, body->GetLinearVelocity().y));
		//	if (Wizzard::Input::IsKeyDown(Wizzard::Key::A))
		//		//transform.Translation.x -= 500.0f * timeStep;
		//		body->SetLinearVelocity(b2Vec2(-100.0f * timeStep, body->GetLinearVelocity().y));
		//
		//	if (!ccc.disableGravity)
		//	{
		//		body->SetGravityScale(1.0f);
		//
		//		if (ccc.canJump)
		//		{
		//			if (Wizzard::Input::IsKeyDown(Wizzard::Key::Space))
		//			{
		//				body->ApplyForceToCenter(b2Vec2(0.0f, 2000.0f * timeStep), true);
		//				//ccc.canJump = false;
		//			}
		//		}
		//
		//		if (body->GetLinearVelocity().y > -0.001f && body->GetLinearVelocity().y < 0.001f)
		//			ccc.canJump = true;
		//	}
		//	else
		//		body->SetGravityScale(0.0f);
		//}
	}
}

void GameApplicationLayer::OnImGuiRender()
{
}

void GameApplicationLayer::OnEvent(Wizzard::Event& event)
{
}