#pragma once

#include "../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class MainMenuScene : public CombatTrackerLib::Scene {
	public:
		explicit MainMenuScene(CombatTrackerLib::App* app);
		~MainMenuScene();

		virtual void notifyScene(const ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;

	private:

		// Flags.
		ImGuiDockNodeFlags m_mainMenuDockspaceFlags{};
		ImGuiWindowFlags m_hostWindowFlags{};

		// Sort of a flag that overrides some options that are enforced on every window.
		// Used for the window that hosts our application art.
		ImGuiWindowClass m_mainMenuWindowClass{};

		// UI IDs.
		ImGuiID m_mainMenuDockspaceID{};
		ImGuiID m_encounterCreationWindowID{};
		ImGuiID m_encounterRunningWindowID{};
		ImGuiID m_completedEncountersWindowID{};
		ImGuiID m_mainMenuTitleWindowID{};

		// Dockspace layout flag.
		bool m_isMainMenuDockspaceInitialized{};
	};

}