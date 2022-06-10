//@BridgetACasey

#pragma once

#include "Core.h"
#include "../event/Event.h"

namespace Wizzard
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return debugName; }

	protected:
		std::string debugName;
	};
}