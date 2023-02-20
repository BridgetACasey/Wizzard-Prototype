//@BridgetACasey

#pragma once

#include "wizzard/core/Layer.h"

#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/KeyEvent.h"
#include "wizzard/event/MouseEvent.h"
#include "wizzard/event/UIEvent.h"
#include "wizzard/event/EventHandler.h"

namespace Wizzard
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();

		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

		void BlockImGuiEvents(bool shouldBlock) { blockImGuiEvents = shouldBlock; }

	private:
		float time = 0.0f;

		bool blockImGuiEvents = true;
	};
}