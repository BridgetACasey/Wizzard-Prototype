//@BridgetACasey

#include "SceneApp2D.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "wizzard/platform/opengl/OpenGLShader.h"

//Temp values for testing audio playback
static float xPosition = 5.0f;
static float playFrequency = 6.0f;
static float audioTimer = 3.5f;	//Start value

SceneApp2D::SceneApp2D() : Layer("SceneApp2D"), orthoCamController(1920.0f / 1080.0f)
{
	music = Wizzard::AudioSource::LoadFromFile("res/music/examplemusic.mp3", false);
	frontLeft = Wizzard::AudioSource::LoadFromFile("res/sfx/examplesfx-frontleft.ogg", true);
	frontRight = Wizzard::AudioSource::LoadFromFile("res/sfx/examplesfx-frontright.ogg", true);
	moving = Wizzard::AudioSource::LoadFromFile("res/sfx/examplesfx-moving.ogg", true);
}

void SceneApp2D::OnAttach()
{
	customTexture = Wizzard::Texture2D::Create("res/textures/kate-derp.png");

	if(music.IsLoaded())
	{
		music.SetLoop(true);
		Wizzard::Audio::Play(music);
	}

	if(frontLeft.IsLoaded())
	{
		frontLeft.SetGain(5.0f);
		frontLeft.SetPosition(-5.0f, 0.0f, 5.0f);
		Wizzard::Audio::Play(frontLeft);
	}

	if(frontRight.IsLoaded())
	{
		frontRight.SetGain(5.0f);
		frontRight.SetPosition(5.0f, 0.0f, 5.0f);
		Wizzard::Audio::Play(frontRight);
	}

	if(moving.IsLoaded())
	{
		moving.SetGain(5.0f);
		moving.SetPosition(5.0f, 0.0f, 5.0f);
	}
}

void SceneApp2D::OnDetach()
{
}

void SceneApp2D::OnUpdate(Wizzard::Timestep timeStep)
{
	orthoCamController.OnUpdate(timeStep);

	if(audioTimer < 0.0f)
	{
		xPosition = 5.0f;
		Wizzard::Audio::Play(moving);
		audioTimer = playFrequency;
	}

	xPosition -= timeStep.GetSeconds() * 2.0f;
	audioTimer -= timeStep.GetSeconds();

	moving.SetPosition(xPosition, 0.0f, 5.0f);

	Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Wizzard::RenderCommand::Clear();

	Wizzard::Renderer2D::BeginScene(orthoCamController.GetCamera());
	Wizzard::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Wizzard::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Wizzard::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, customTexture);
	Wizzard::Renderer2D::EndScene();

	Wizzard::Renderer::EndScene();
}

void SceneApp2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void SceneApp2D::OnEvent(Wizzard::Event& event)
{
	orthoCamController.OnEvent(event);
}