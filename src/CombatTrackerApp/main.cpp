#include "../CombatTrackerLib/App.h"
#include "./Scenes/OverlayScene.h"
#include "./Scenes/MainMenuScene.h"
#include "imgui.h"

int main() {
	CombatTrackerLib::WindowSpecification windowSpec{ "CombatTrackerMainWindow", 1920, 1080, true, true };
	CombatTrackerLib::AppSpecification appSpec{windowSpec, "CombatTracker", true};
	CombatTrackerLib::App application(appSpec);
	application.queueAddScene(std::make_unique<CombatTrackerApp::MainMenuScene>(&application));
	application.queueAddScene(std::make_unique<CombatTrackerApp::OverlayScene>(&application));
	application.run();
}