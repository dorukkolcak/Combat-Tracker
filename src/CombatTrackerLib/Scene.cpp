#include "Scene.h"

namespace CombatTrackerLib {
	Scene::Scene(App* app) : applicationReference(app)
	{
	}
    // Taken from https://github.com/ocornut/imgui/discussions/3862.
    bool Scene::centeredButton(const char* label) {

        ImGuiStyle& style = ImGui::GetStyle();
        float buttonSize = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;

        float off = (avail - buttonSize) * 0.5f;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        return ImGui::Button(label);
    }
}