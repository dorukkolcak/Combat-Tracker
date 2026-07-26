#pragma once

#include "../../CombatTrackerLib/Scene.h"
#include "../../CombatTrackerLib/Texture2D.h"

namespace CombatTrackerApp {
	class MainMenuScene : public CombatTrackerLib::Scene {
	public:
		explicit MainMenuScene(CombatTrackerLib::App* app);

		void notifyScene(const ImGuiIO& imguiIO) override;
		void updateScene(double timeInterval) override;
		void renderUI() override;
		void renderScene() override;

	private:

		CombatTrackerLib::Texture2D m_titleTexture;

		// Sort of a flag that overrides some options that are enforced on every window.
		// Used for the window that hosts our application art.
		ImGuiWindowClass m_mainMenuWindowClass{};

		// Flags.
		ImGuiDockNodeFlags m_mainMenuDockspaceFlags{};
		ImGuiWindowFlags m_hostWindowFlags{};

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