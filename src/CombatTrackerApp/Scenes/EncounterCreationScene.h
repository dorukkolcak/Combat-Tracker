#pragma once

#include "../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {
	class EncounterCreationScene : public CombatTrackerLib::Scene {
	public:
		EncounterCreationScene(CombatTrackerLib::App* app);
		virtual ~EncounterCreationScene();

		virtual void notifyScene(ImGuiIO& imguiIO) override;
		virtual void updateScene(double timeInterval) override;
		virtual void renderUI() override;
		virtual void renderScene() override;
	private:
		// Flags.
		ImGuiWindowFlags encounterCreationWindowFlags{};
		ImGuiDockNodeFlags encounterCreationDockFlags{};

		// High level windows.
		ImGuiID encounterManagerDockspaceID{};
		ImGuiID encounterManagerWindowID{};
		ImGuiID entityBrowserWindowID{};
		ImGuiID buttonsWindowID{};

		//Entity Browser lower level windows.
		ImGuiID entityBrowserDockspaceID{};
		ImGuiID monsterBrowserWindowID{};
		ImGuiID playerCharacterBrowserWindowID{};
		ImGuiID encounterBrowserWindowID{};

		// Dockspace layout flags.
		bool isEncounterManagerDockspaceInitialized;
		bool isEntityBrowserDockspaceInitialized;
	};
}