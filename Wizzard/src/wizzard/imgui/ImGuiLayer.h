//@BridgetACasey

//#pragma once
//
//#include "wizzard/core/Layer.h"
//
//#include "wizzard/event/ApplicationEvent.h"
//#include "wizzard/event/KeyEvent.h"
//#include "wizzard/event/MouseEvent.h"
//#include "wizzard/event/EventHandler.h"
//
//namespace Wizzard
//{
//	class ImGuiLayer : public Layer
//	{
//	public:
//		ImGuiLayer();
//		~ImGuiLayer();
//
//		void OnAttach() override;
//		void OnDetach() override;
//		void OnImGuiRender() override;
//
//		void OnUpdate() override;
//		void OnEvent(Event& event) override;
//
//		void Begin();
//		void End();
//
//	private:
//		//bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& mouseEvent);
//		//bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& mouseEvent);
//		//bool OnMouseMovedEvent(MouseMovedEvent& mouseEvent);
//		//bool OnMouseScrolledEvent(MouseScrolledEvent& mouseEvent);
//		//
//		//bool OnKeyPressedEvent(KeyPressedEvent& keyEvent);
//		//bool OnKeyReleasedEvent(KeyReleasedEvent& keyEvent);
//		//bool OnKeyTypedEvent(KeyTypedEvent& keyEvent);
//		//bool OnWindowResizeEvent(WindowResizeEvent& keyEvent);
//
//		float time = 0.0f;
//	};
//}