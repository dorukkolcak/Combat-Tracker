#include "../CombatTrackerLib/App.h"
#include "./Scenes/OverlayScene.h"
#include "./Scenes/MainMenuScene.h"
#include "imgui.h"

int main() {
	// Setup Dear ImGui context. With how the data is managed, context creation should occur precisely before app initiliazation.
	CombatTrackerLib::WindowSpecification windowSpec(1000, 800, "CombatTrackerMainWindow", true, true);
	CombatTrackerLib::AppSpecification appSpec("CombatTracker", windowSpec);
	CombatTrackerLib::App application(appSpec);
	application.queueAddScene(std::make_unique<CombatTrackerApp::MainMenuScene>(&application));
	application.queueAddScene(std::make_unique<CombatTrackerApp::OverlayScene>(&application));
	application.dequeueSceneOperations(); // Don't forget to dequeue at the start of application.
	application.run();
}