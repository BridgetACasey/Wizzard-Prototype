//@BridgetACasey

#include <wzpch.h>

#include "ObjectPropertiesPanel.h"

#include "imgui.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

namespace Wizzard
{
	void ObjectPropertiesPanel::OnImGuiRender()
	{
		ImGuiSR::WindowBegin("Properties");

		ImGui::End();
	}
}