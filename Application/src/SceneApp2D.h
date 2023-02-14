//@BridgetACasey

#pragma once

#include "Wizzard.h"

class SceneApp2D : public Wizzard::Layer
{
public:
	SceneApp2D();
	virtual ~SceneApp2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Wizzard::Timestep timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Wizzard::Event& event) override;

private:
	Wizzard::OrthographicCameraController orthoCamController;
};