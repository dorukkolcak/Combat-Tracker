#pragma once

#include "../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class MainMenuScene : public CombatTrackerLib::Scene {
	public:
		MainMenuScene(CombatTrackerLib::App* app);
		~MainMenuScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;

	private:
		// Flags.
		ImGuiDockNodeFlags mainMenuDockspaceFlags{};
		ImGuiWindowFlags hostWindowFlags{};

		// Sort of a flag that overrides some options that are enforced on every window.
		// Used for the window that hosts our application art.
		ImGuiWindowClass mainMenuWindowClass{};

		// UI IDs.
		ImGuiID mainMenuDockspaceID{};
		ImGuiID encounterCreationWindowID{};
		ImGuiID encounterRunningWindowID{};
		ImGuiID completedEncountersWindowID{};
		ImGuiID mainMenuTitleWindowID{};

		// Dockspace layout flag.
		bool isMainMenuDockspaceInitialized;
	};

}