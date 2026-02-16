#include "OverlayScene.h"
#include "imgui_impl_opengl3.h"

namespace CombatTrackerApp {
	OverlayScene::OverlayScene(CombatTrackerLib::App* app) : CombatTrackerLib::Scene(app)
	{
	}
	OverlayScene::~OverlayScene() {}

	void OverlayScene::notifyScene(ImGuiIO& imguiIO) {}
	void OverlayScene::updateScene(double timeInterval) {}
	void OverlayScene::renderUI() {
		ImGui::ShowDebugLogWindow();
	}
	void OverlayScene::renderScene() {}
}