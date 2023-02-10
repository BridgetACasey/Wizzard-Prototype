//@BridgetACasey

#pragma once

#include "Core.h"
#include "Timestep.h"
#include "wizzard/event/Event.h"

namespace Wizzard
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return debugName; }

	protected:
		std::string debugName;
	};
}