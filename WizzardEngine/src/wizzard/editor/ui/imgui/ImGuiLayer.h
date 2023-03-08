//@BridgetACasey

#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS	//TODO: Move this to a more appropriate location

#include "wizzard/common/Layer.h"

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

		void SetLogElementMessage(bool log) { logElementMessage = log; }
		bool GetLogElementMessage() const { return logElementMessage; }

		void SetLogWindowMessage(bool log) { logWindowMessage = log; }
		bool GetLogWindowMessage() const { return logWindowMessage; }

		unsigned int GetUIElementMessageID() const { return uiElementMessageID; }
		unsigned int GetUIWindowMessageID() const { return uiWindowMessageID; }

	private:
		bool OnUIWindowHoveredEvent(UIWindowHoveredEvent& uiWindowHoveredEvent);
		bool OnUIWindowFocusEvent(UIWindowFocusEvent& uiWindowFocusEvent);
		bool OnUIElementHovered(UIElementHoveredEvent& uiElementHoveredEvent);
		bool OnUIElementSelected(UIElementSelectedEvent& uiElementSelectedEvent);

		float time = 0.0f;

		bool blockImGuiEvents = true;

		bool logElementMessage = false;
		bool logWindowMessage = false;
		unsigned int uiElementMessageID;
		unsigned int uiWindowMessageID;

		std::string imguiIniFilePath;
		std::string imguiLogFilePath;
	};
}