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
		ImGuiDockNodeFlags mainMenuDockspaceFlags{};
		ImGuiWindowFlags hostWindowFlags{};

		ImGuiID mainMenuDockspaceID{};
		ImGuiID encounterCreationWindowID{};
		ImGuiID encounterRunningWindowID{};
		ImGuiID completedEncountersWindowID{};
		ImGuiID mainMenuTitleWindowID{};

	};

}