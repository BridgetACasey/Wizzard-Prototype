//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	struct PanelData
	{
		const char* id = "";
		const char* name = "";
		WizRef<EditorPanel> panel = nullptr;
	};

	class PanelManager
	{
	public:
		PanelManager() = default;
		~PanelManager();

		void OnEvent(Event& event);
		void OnImGuiRender();

		void SetSceneContext(const WizRef<Scene>& context);

		PanelData GetPanelData(const char* panelID);
		std::unordered_map<const char*, PanelData>& GetPanels() { return panels; }

		void RemovePanel(const char* panelID);

		template<typename TPanel>
		WizRef<TPanel> AddPanel(const PanelData& panelData)
		{
			if (panels.find(panelData.id) != panels.end())
			{
				WIZ_ERROR("A panel with this id already exists");
				return nullptr;
			}

			panels[panelData.id] = panelData;
			return panelData.panel.GetAs<TPanel>();
		}

		template<typename TPanel, typename... TArgs>
		WizRef<TPanel> AddPanel(const char* nameID, const char* displayName, TArgs&&... args)
		{
			return AddPanel<TPanel>(PanelData{ nameID, displayName, WizRef<TPanel>::CreateRef(std::forward<TArgs>(args)...) });
		}

	private:
		std::unordered_map<const char*, PanelData> panels;
	};
}