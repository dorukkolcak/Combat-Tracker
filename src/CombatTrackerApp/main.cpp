#include "CombatTrackerLib/App.h"
#include "CombatTrackerApp/Scenes/OverlayScene.h"
#include "CombatTrackerApp/Scenes/MainMenuScene.h"

int main() {
	CombatTrackerLib::WindowSpecification windowSpec{ "CombatTrackerMainWindow", 1920, 1080, true, true };
	CombatTrackerLib::App::Specification appSpec{CombatTrackerLib::UIPreset::DARK, std::move(windowSpec), "CombatTracker"};
	CombatTrackerLib::App application(appSpec);
	application.queueAddScene(std::make_unique<CombatTrackerApp::MainMenuScene>(&application));
	application.queueAddScene(std::make_unique<CombatTrackerApp::OverlayScene>(&application));
	application.run();
}