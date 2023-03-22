//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/event/EditorEvent.h"
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class SceneHierarchyPanel : public EditorPanel
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		SceneHierarchyPanel() = default;

		void OnImGuiRender() override;

		void OnRenderPropertiesPanel();	//TODO: Temp, move to separate panel class later

		Entity GetSelectedEntity() const { return selectionContext; }
		void SetSelectedEntity(Entity entity);
		void SetSelectedEntityToDefault();

		void SetEventCallback(const EventCallbackFunction& callback) { selectionData.eventCallback = callback; }

	private:
		struct SelectionData
		{
			EventCallbackFunction eventCallback;
		};

		bool OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent);

		void DrawComponents(Entity entity);
		void DrawEntityNode(Entity entity);

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		Entity selectionContext = {};
		Entity defaultEntity = {};

		SelectionData selectionData;
	};
}