#include "EncounterCreationScene.h"
#include "MainMenuScene.h"
#include "MonsterCreationScene.h"
#include "PlayerCharacterCreationScene.h"

#include "imgui_internal.h"

#include "imgui_stdlib.h"

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		m_monsterSource{ EntitySource::ALL },
		m_playerCharacterSource{ EntitySource::ALL },
		m_selectedMonsterIndex{ -1 },
		m_selectedPlayerCharacterIndex{ -1 },
		m_isEncounterCreatorDockspaceInitialized{ false }
	{}

	void EncounterCreationScene::notifyScene(const ImGuiIO& imguiIO) {}
	void EncounterCreationScene::updateScene(double timeInterval) {}

	void EncounterCreationScene::renderUI() {

		// The menu bar is rendered inside the host window and the three browsers inside the entity browser's tab bar.
		// So only the three top level windows are submitted from here.
		renderHostWindow();
		renderEntityBrowserHostWindow();
		renderEncounterManagerWindow();

	}
	void EncounterCreationScene::renderScene() {}

	void EncounterCreationScene::renderHostWindow() {

		using enum WindowType;

		// Set configurations of the host window of the scene on first render pass.
		if (!m_isEncounterCreatorDockspaceInitialized) {
			m_mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(m_mainViewport->WorkPos);
			ImGui::SetNextWindowSize(m_mainViewport->WorkSize);
			ImGui::SetNextWindowViewport(m_mainViewport->ID);
		}

		// Setup dockspace layout on first render pass.
		if (!m_isEncounterCreatorDockspaceInitialized) {

			m_encounterCreatorDockspaceID =
				ImGui::GetID(
					(MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR) + " - Dockspace")
					.c_str()
				);
			ImGui::DockBuilderRemoveNode(m_encounterCreatorDockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(m_encounterCreatorDockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(m_encounterCreatorDockspaceID, m_mainViewport->WorkSize);

			// Configure dockspace nodes.
			ImGui::DockBuilderSplitNode(
				m_encounterCreatorDockspaceID,
				ImGuiDir_Left,
				0.38f,
				&m_entityBrowserWindowID,
				&m_encounterManagerWindowID
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str(),
				m_entityBrowserWindowID
			);
			ImGui::DockBuilderDockWindow(
				MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str(),
				m_encounterManagerWindowID
			);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(m_encounterCreatorDockspaceID);
			m_isEncounterCreatorDockspaceInitialized = true;
		}

		// Render host window.
		ImGui::Begin(
			MAPPED_WINDOW_NAMES.at(ENCOUNTER_CREATOR).c_str(),
			nullptr,
			ENCOUNTER_CREATION_WINDOW_FLAGS
		);

		// The menu bar of the host window. This should grant access to functionality such as returning to the main menu.
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu(MAPPED_WINDOW_NAMES.at(WindowType::MENU).c_str())) {
				if (ImGui::MenuItem("Return to Main Menu")) {
					m_applicationReference->queueSceneTransition(this, std::make_unique<MainMenuScene>(m_applicationReference));
				}
				// Can be expanded from here.
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::DockSpace(
			m_encounterCreatorDockspaceID,
			ImGui::GetContentRegionAvail(),
			ENCOUNTER_CREATION_DOCKSPACE_FLAGS
		);
		ImGui::End();
	}
	void EncounterCreationScene::renderEntityBrowserHostWindow() {

		using enum WindowType;

		// This window hosts each entity's seperate subbrowser window.
		// These subbrowser windows are accessed via a tab bar.
		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENTITY_BROWSER).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);

		// Tabs of the window host other windows for each entity (Monster and PlayerCharacter).
		if (ImGui::BeginTabBar("##EntityBrowserTabs")) {
			if (ImGui::BeginTabItem(MAPPED_WINDOW_NAMES.at(MONSTER_BROWSER).c_str())) {
				renderEntityBrowserWindow<2, MonsterCreationScene>(
					"Monster",
					MONSTER_BROWSER_COLUMN_LABELS,
					m_monsterSource,
					m_monsterSearchQuery,
					m_selectedMonsterIndex
				);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(MAPPED_WINDOW_NAMES.at(PLAYER_CHARACTER_BROWSER).c_str())) {
				renderEntityBrowserWindow<3, PlayerCharacterCreationScene>(
					"Player Character",
					PLAYER_CHARACTER_BROWSER_COLUMN_LABELS,
					m_playerCharacterSource,
					m_playerCharacterSearchQuery,
					m_selectedPlayerCharacterIndex
				);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	void EncounterCreationScene::renderBrowserFilters(EntitySource& source, std::string& searchQuery, const char* label) {

		// Each tab passes its own label so that the widgets below don't collide in the ID stack.
		ImGui::PushID(label);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted("Source");
		ImGui::SameLine();
		if (ImGui::RadioButton("All", source == EntitySource::ALL)) {
			source = EntitySource::ALL;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("User Created", source == EntitySource::USER_CREATED)) {
			source = EntitySource::USER_CREATED;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Compendium", source == EntitySource::COMPENDIUM)) {
			source = EntitySource::COMPENDIUM;
		}
		ImGui::InputTextWithHint("##Search", "Search by name", &searchQuery);
		ImGui::PopID();
	}

	// TODO: Finish this window when all monster and player character functionality is done.
	// This window is rendered as the "right" node of the dockspace.
	void EncounterCreationScene::renderEncounterManagerWindow() {

		using enum WindowType;

		ImGui::Begin(MAPPED_WINDOW_NAMES.at(ENCOUNTER_MANAGER).c_str());
		ImGui::End();
	}
}
