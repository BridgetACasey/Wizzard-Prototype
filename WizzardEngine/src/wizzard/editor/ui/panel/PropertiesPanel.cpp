//@BridgetACasey

#include <wzpch.h>

#include "PropertiesPanel.h"

#include "imgui.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

namespace Wizzard
{
	void PropertiesPanel::OnImGuiRender()
	{
		ImGuiSR::Begin("PROPERTIES");

		ImGui::End();
	}
}