#include "EncounterCreationScene.h"
#include "MainMenuScene.h"
#include "imgui_internal.h"

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) : 
		CombatTrackerLib::Scene(app)
	{
		encounterCreationDockFlags = 
			ImGuiDockNodeFlags_NoResize |
			ImGuiDockNodeFlags_NoSplit;
		encounterCreationWindowFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;
	}
	EncounterCreationScene::~EncounterCreationScene() {}

	void EncounterCreationScene::notifyScene(ImGuiIO& imguiIO) {}
	void EncounterCreationScene::updateScene(double timeInterval) {}
	void EncounterCreationScene::renderUI() {
	
		using EncounterCreationWindows::ENCOUNTER_CREATOR;
		using EncounterCreationWindows::MENU;
		using EncounterCreationWindows::ENTITY_BROWSER;
		using EncounterCreationWindows::ENCOUNTER_MANAGER;
		using EncounterCreationWindows::MONSTER_BROWSER;
		using EncounterCreationWindows::PLAYER_CHARACTER_BROWSER;
		using EncounterCreationWindows::ENCOUNTER_BROWSER;

		// Set configurations of the host window of the scene on first render pass.
		if (!dockspaceInitializations.at(ENCOUNTER_CREATOR)) {
			mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(mainViewport->WorkPos);
			ImGui::SetNextWindowSize(mainViewport->WorkSize);
			ImGui::SetNextWindowViewport(mainViewport->ID);
		}

		// Render host window.
		ImGui::Begin(
			MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR).c_str(),
			nullptr, 
			encounterCreationWindowFlags
		);

		// Setup dockspace layout on first render pass.
		if (!dockspaceInitializations.at(ENCOUNTER_CREATOR)) {

			dockspaceIDs[ENCOUNTER_CREATOR] = 
				ImGui::GetID(
					(MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR) + " - Dockspace")
					.c_str()
				);
			ImGui::DockBuilderRemoveNode(dockspaceIDs.at(ENCOUNTER_CREATOR));

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(dockspaceIDs.at(ENCOUNTER_CREATOR), ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceIDs.at(ENCOUNTER_CREATOR), mainViewport->WorkSize);

			// Configure dockspace nodes.
			ImGuiID leftID;
			ImGui::DockBuilderSplitNode(
				dockspaceIDs.at(ENCOUNTER_CREATOR), 
				ImGuiDir_Left, 
				0.10f, 
				&(dockspaceIDs[MENU]), 
				&leftID
			);
			ImGui::DockBuilderSplitNode(leftID, 
				ImGuiDir_Left, 
				0.36f, 
				&(dockspaceIDs[ENTITY_BROWSER]), 
				&(dockspaceIDs[ENCOUNTER_MANAGER])
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str(), 
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str(), 
				dockspaceIDs.at(ENCOUNTER_MANAGER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(MENU).c_str(),
				dockspaceIDs.at(MENU)
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(dockspaceIDs.at(ENCOUNTER_CREATOR));
			dockspaceInitializations.at(ENCOUNTER_CREATOR) = true;
		}
		// Render dockspace of the host window.
		ImGui::DockSpace(
			dockspaceIDs.at(ENCOUNTER_CREATOR),
			ImGui::GetWindowSize(),
			encounterCreationDockFlags
		);
		ImGui::End();

		// Submit windows.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(MENU).c_str());
		if (ImGui::Button("Return to Main Menu")) {
			applicationReference->queueSceneTransition(this, std::make_unique<MainMenuScene>(applicationReference));
		}
		ImGui::End();

		/*
		Here is the overall layout of this window:
		The window will have 3 tabs: Monsters, Player Characters and Encounters.
		Each tab is further subdivided into two parts: User Created and Compendium.
		*/
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str());
		if (!dockspaceInitializations.at(ENTITY_BROWSER)) {

			dockspaceIDs[ENTITY_BROWSER] =
				ImGui::GetID(
					(MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER) + " - Dockspace")
					.c_str()
				);
			ImGui::DockBuilderRemoveNode(dockspaceIDs.at(ENTITY_BROWSER));

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(dockspaceIDs.at(ENTITY_BROWSER), ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceIDs.at(ENTITY_BROWSER), ImGui::GetWindowSize());

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str(), 
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER).c_str(),
				dockspaceIDs.at(ENTITY_BROWSER)
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(dockspaceIDs.at(ENTITY_BROWSER));
			dockspaceInitializations.at(ENTITY_BROWSER) = true;
		}
		// Render dockspace of the entity browser window.
		ImGui::DockSpace(dockspaceIDs.at(ENTITY_BROWSER), ImGui::GetWindowSize());
		ImGui::End();
		
		// Monster browser window.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(MONSTER_BROWSER)) {
			initializeSubBrowserDockspace(MONSTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(MONSTER_BROWSER), ImGui::GetWindowSize());
		ImGui::End();
		
		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
		
		// Player character browser window.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(PLAYER_CHARACTER_BROWSER)) {
			initializeSubBrowserDockspace(PLAYER_CHARACTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(PLAYER_CHARACTER_BROWSER), ImGui::GetWindowSize());
		ImGui::End();
		
		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
		
		// Encounter browser window.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER).c_str());
		if (!dockspaceInitializations.at(ENCOUNTER_BROWSER)) {
			initializeSubBrowserDockspace(ENCOUNTER_BROWSER);
		}
		ImGui::DockSpace(dockspaceIDs.at(ENCOUNTER_BROWSER), ImGui::GetWindowSize());
		ImGui::End();
		
		// TODO: Finalize UI design after "backend" stuff is done.
		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - User Created").c_str());

		ImGui::End();

		ImGui::Begin((MAPPED_WINDOW_NAMES.at(ENCOUNTER_BROWSER) + " - Compendium").c_str());

		ImGui::End();
		
		// TODO: Encounter manager window. Finalize UI design after "backend" stuff is done.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str());
		ImGui::End();
	}
	void EncounterCreationScene::renderScene() {}

	void EncounterCreationScene::initializeSubBrowserDockspace(EncounterCreationWindows windowType) {
		dockspaceIDs[windowType] =
			ImGui::GetID(
				(MAPPED_WINDOW_NAMES.at(windowType) + "- Dockspace")
				.c_str()
		);
		
		auto& id = dockspaceIDs.at(windowType);

		ImGui::DockBuilderRemoveNode(id);

		// Add dockspace nodes.
		ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(id, ImGui::GetWindowSize());
		
		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - User Created").c_str(), id);
		ImGui::DockBuilderDockWindow((MAPPED_WINDOW_NAMES.at(windowType) + " - Compendium").c_str(), id);

		ImGui::DockBuilderFinish(id);
		dockspaceInitializations.at(windowType) = true;
	}
}