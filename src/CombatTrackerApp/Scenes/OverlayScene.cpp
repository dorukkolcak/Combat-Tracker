#include "CombatTrackerApp/Scenes/OverlayScene.h"

namespace CombatTrackerApp {
	OverlayScene::OverlayScene(CombatTrackerLib::App* app) : 
		CombatTrackerLib::Scene{ app }
	{
	}

	void OverlayScene::notifyScene(const ImGuiIO&) {} 
	void OverlayScene::updateScene(double) {}
	void OverlayScene::renderUI() {

		/*
		DEBUG AND DEVELOPMENT ONLY, remove these later.
		It is pretty cumbersome to progmatically dock these windows. 
		As these windows are submitted before dockspace creation.
		So we essentially need to create identical copies that are registered after dockspace creation. 
		I can't be bothered to do this. Instead we submit them during program execution.
		This change is saved in our .ini file so we only need to do this once.
		*/
		ImGui::ShowDebugLogWindow();
		ImGui::ShowMetricsWindow();
		ImGui::ShowDemoWindow();

	}
	void OverlayScene::renderScene() {}
}