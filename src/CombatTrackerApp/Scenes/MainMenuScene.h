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
		ImGuiID mainMenuDockspaceID{};
		ImGuiViewport* mainMenuViewport = nullptr;

		ImGuiDockNodeFlags mainMenuDockspaceFlags;
		ImGuiWindowFlags hostWindowFlags{};

		ImGuiID encounterCreationID{};
		ImGuiID encounterRunningID{};
		ImGuiID completedEncountersViewID{};

	};

}