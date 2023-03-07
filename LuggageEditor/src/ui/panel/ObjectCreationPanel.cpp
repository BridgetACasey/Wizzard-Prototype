//@BridgetACasey

#include "ObjectCreationPanel.h"

#include "imgui/imgui.h"
#include "wizzard/scene/Entity.h"
#include "wizzard/scene/component/SpriteComponent.h"
#include "wizzard/scene/component/TransformComponent.h"
#include "wizzard/ui/ImGuiScreenReading.h"

namespace Wizzard
{
    ObjectCreationPanel::ObjectCreationPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }

    void ObjectCreationPanel::SetContext(const Ref<Scene>& scene)
    {
        sceneContext = scene;
    }

    void ObjectCreationPanel::OnImGuiRender()
    {
        ImGuiSR::WindowBegin("Object Menu");

        ImGui::TextWrapped("Create new objects here!");

        if (ImGuiSR::Button("ADD SQUARE", ImVec2(400.0f, 80.5f)))
        {
            auto square = sceneContext->CreateEntity("Red Square");
            square.GetComponent<TransformComponent>().Translation.x = -1.5f;
            square.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
        }

        ImGui::End();
    }
}