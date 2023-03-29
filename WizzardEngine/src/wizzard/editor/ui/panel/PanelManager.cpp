//@BridgetACasey

#include "wzpch.h"

#include "PanelManager.h"

namespace Wizzard
{
	PanelManager::~PanelManager()
	{
		panels.clear();
	}

	void PanelManager::OnEvent(Event& event)
	{
		for (auto& [id, panelData] : panels)
		{
			panelData.panel->OnEvent(event);
		}
	}

	void PanelManager::OnImGuiRender()
	{
		for (auto& [id, panelData] : panels)
		{
			panelData.panel->OnImGuiRender();
		}
	}

	void PanelManager::SetSceneContext(const WizRef<Scene>& context)
	{
		for (auto& [id, panelData] : panels)
			panelData.panel->SetSceneContext(context);
	}

	void PanelManager::CycleActivePanel()
	{
		focusedPanelIndex = (focusedPanelIndex >= (panels.size() - 1)) ? 0 : focusedPanelIndex+1;

		std::next(panels.begin(), focusedPanelIndex)->second.panel->SetShouldTriggerFocus(true);
	}

	PanelData PanelManager::GetPanelData(const char* panelID)
	{
		return panels.at(panelID);
	}

	void PanelManager::RemovePanel(const char* panelID)
	{
		if (panels.find(panelID) == panels.end())
			panels.erase(panelID);
		else
			WIZ_ERROR("Panel manager could not find panel with ID {0}", panelID);
	}
}