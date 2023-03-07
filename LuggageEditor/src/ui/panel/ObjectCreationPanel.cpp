//@BridgetACasey

#include "ObjectCreationPanel.h"

#include "imgui/imgui.h"

namespace Wizzard
{
    void ObjectCreationPanel::OnImGuiRender()
    {
        ImGui::Begin("Object Menu");
        ImGui::TextWrapped("Create new objects here!");
        ImGui::End();
    }
}