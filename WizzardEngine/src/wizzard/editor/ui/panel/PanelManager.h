//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "event/Event.h"
#include "scene/Scene.h"

//TODO

namespace Wizzard
{
	struct PanelData
	{
		const char* id = "";
		const char* name = "";
		Ref<EditorPanel> panel = nullptr;
		bool isOpen = false;
	};

	enum class PanelCategory
	{
		Edit, View, TotalCount
	};

	class PanelManager
	{
	public:
		PanelManager() = default;
		~PanelManager();

		void SetSceneContext(const Ref<Scene>& sceneContext);
		void OnEvent(Event& event);
		void OnImGuiRender();

		void RemovePanel(const char* panelID);

	private:
		std::array<std::unordered_map<uint32_t, PanelData>, (size_t)PanelCategory::TotalCount> panels;
	};
}