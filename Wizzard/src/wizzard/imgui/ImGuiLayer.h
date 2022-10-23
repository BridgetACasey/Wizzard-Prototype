//@BridgetACasey

#pragma once

#include "../core/Layer.h"

namespace Wizzard
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float time = 0.0f;
	};
}