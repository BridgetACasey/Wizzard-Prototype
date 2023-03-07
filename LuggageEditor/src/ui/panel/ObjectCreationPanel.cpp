//@BridgetACasey

#include "ObjectCreationPanel.h"

#include "imgui/imgui.h"
#include "wizzard/ui/ImGuiScreenReading.h"

namespace Wizzard
{
    void ObjectCreationPanel::OnImGuiRender()
    {
        ImGuiSR::WindowBegin("Object Menu");
        ImGui::TextWrapped("Create new objects here!");
        ImGui::End();
    }
}