#include "EncounterRunningScene.h"

namespace CombatTrackerApp {
	EncounterRunningScene::EncounterRunningScene(CombatTrackerLib::App* app) : CombatTrackerLib::Scene(app)
	{
	}
	EncounterRunningScene::~EncounterRunningScene() {}

	void EncounterRunningScene::notifyScene(ImGuiIO& imguiIO) {}
	void EncounterRunningScene::updateScene(double timeInterval) {}
	void EncounterRunningScene::renderUI() {}
	void EncounterRunningScene::renderScene() {}
}