//@BridgetACasey

#pragma once

#include <Wizzard.h>
#include <wizzard/base/Reference.h>

class GameApplicationLayer : public Wizzard::Layer
{
public:
	GameApplicationLayer();
	virtual ~GameApplicationLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Wizzard::TimeStep timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Wizzard::Event& event) override;

private:
	Wizzard::WizRef<Wizzard::Scene> gameScene;

	Wizzard::Entity playerEntity;
};