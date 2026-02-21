#pragma once

#include <string>

#include "../../CombatTrackerLib/Scene.h"

namespace CombatTrackerApp {

	// Ordered from high level windows to low level windows. 
	enum class EncounterCreationWindows {
		ENCOUNTER_CREATOR,
		MENU,
		ENTITY_BROWSER,
		ENCOUNTER_MANAGER,
		MONSTER_BROWSER,
		PLAYER_CHARACTER_BROWSER,
		ENCOUNTER_BROWSER
	};

	using EncounterCreationWindows::ENCOUNTER_CREATOR;
	using EncounterCreationWindows::MENU;
	using EncounterCreationWindows::ENTITY_BROWSER;
	using EncounterCreationWindows::ENCOUNTER_MANAGER;
	using EncounterCreationWindows::MONSTER_BROWSER;
	using EncounterCreationWindows::PLAYER_CHARACTER_BROWSER;
	using EncounterCreationWindows::ENCOUNTER_BROWSER;

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
		
		// Dockspace variables.
		std::unordered_map<EncounterCreationWindows, ImGuiID> dockspaceIDs;
		std::unordered_map<EncounterCreationWindows, bool> dockspaceInitializations{
			{ENCOUNTER_CREATOR, false},
			{ENTITY_BROWSER, false},
			{MONSTER_BROWSER, false},
			{PLAYER_CHARACTER_BROWSER, false},
			{ENCOUNTER_BROWSER, false}
		};

		const std::unordered_map<EncounterCreationWindows, std::string> MAPPED_WINDOW_NAMES{
			{ENCOUNTER_CREATOR, "Encounter Creator"},
			{ENCOUNTER_MANAGER, "Encounter Manager"},
			{MENU, "Menu"},
			{ENTITY_BROWSER, "Entity Browser"},
			{MONSTER_BROWSER, "Monster Browser"},
			{PLAYER_CHARACTER_BROWSER, "Player Character Browser"},
			{ENCOUNTER_BROWSER, "Encounter Browser"}
		};

		void initializeSubBrowserDockspace(EncounterCreationWindows windowType);
	};
}