#include "CombatTrackerApp/Scenes/EncounterCreationScene.h"
#include "CombatTrackerApp/Scenes/MainMenuScene.h"
#include "CombatTrackerApp/Scenes/MonsterCreationScene.h"
#include "CombatTrackerApp/Scenes/PlayerCharacterCreationScene.h"
#include "CombatTrackerLib/Utilities/GenericUtils.h"

#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace CombatTrackerApp {
	EncounterCreationScene::EncounterCreationScene(CombatTrackerLib::App* app) :
		CombatTrackerLib::Scene{ app },
		m_monsterSource{ EntitySource::ALL },
		m_playerCharacterSource{ EntitySource::ALL },
		m_selectedMonsterIndex{ -1 },
		m_selectedPlayerCharacterIndex{ -1 },
		m_isdockspaceInitialized{ false }
	{}
	void EncounterCreationScene::notifyScene(const ImGuiIO&) {}
	void EncounterCreationScene::updateScene(double) {}
	void EncounterCreationScene::renderUI() {

		// The menu bar is rendered inside the host window and the three browsers inside the entity browser's tab bar.
		// So only the three top level windows are submitted from here.
		renderHostWindow();
		renderEntityBrowserHostWindow();
		renderEncounterManagerWindow();

	}
	void EncounterCreationScene::renderScene() {}
	std::string_view EncounterCreationScene::getWindowName(WindowType windowType) const {
		return m_localeProviderPointer->getTextList("/scenes/encounterCreation/windows")[CombatTrackerLib::enumToSizeT(windowType)];
	}
	void EncounterCreationScene::renderHostWindow() {

		using enum WindowType;
		const auto encounterCreatorWindowName{ getWindowName(ENCOUNTER_CREATOR) };
		const auto entityBrowserWindowName{ getWindowName(ENTITY_BROWSER) };
		const auto encounterManagerWindowName{ getWindowName(ENCOUNTER_MANAGER) };

		// Set configurations of the host window of the scene.
		// This has to happen on every pass and not just the initial one, otherwise the host stays at its old size once the OS window is resized.
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		// Setup dockspace layout on first render pass.
		if (!m_isdockspaceInitialized) {

			m_dockspaceID = ImGui::GetID("Encounter Creator - Dockspace");
			ImGui::DockBuilderRemoveNode(m_dockspaceID);

			// Add dockspace nodes.
			ImGui::DockBuilderAddNode(m_dockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(m_dockspaceID, viewport->WorkSize);

			// Configure dockspace nodes.
			ImGui::DockBuilderSplitNode(
				m_dockspaceID,
				ImGuiDir_Left,
				0.38f,
				&m_entityBrowserWindowID,
				&m_encounterManagerWindowID
			);

			// Dock windows.
			ImGui::DockBuilderDockWindow(entityBrowserWindowName.data(), m_entityBrowserWindowID);
			ImGui::DockBuilderDockWindow(encounterManagerWindowName.data(), m_encounterManagerWindowID);

			// Finish dockspace creation.
			ImGui::DockBuilderFinish(m_dockspaceID);
			m_isdockspaceInitialized = true;
		}

		// Render host window. Like the other host windows, it is just for scaffolding.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		CombatTrackerLib::ScopedWindow window{ encounterCreatorWindowName, nullptr, ENCOUNTER_CREATION_WINDOW_FLAGS };
		ImGui::PopStyleVar(3);

		// The menu bar of the host window. This should grant access to functionality such as returning to the main menu.
		if (CombatTrackerLib::ScopedMenuBar menuBar{  }) {
			if (CombatTrackerLib::ScopedMenu menu{ getWindowName(MENU) }) {
				if (ImGui::MenuItem(m_localeProviderPointer->getText("/scenes/encounterCreation/returnToMainMenu").data())) {
					m_applicationPointer->getSceneManager().queueSceneTransition(this, std::make_unique<MainMenuScene>(m_applicationPointer));
				}
				// TODO: Expand the options from here.
			}
		}

		ImGui::DockSpace(
			m_dockspaceID,
			ImGui::GetContentRegionAvail(),
			DOCKSPACE_FLAGS
		);
	}
	void EncounterCreationScene::renderEntityBrowserHostWindow() {

		using enum WindowType;
		const auto entityBrowserWindowName{ getWindowName(ENTITY_BROWSER) };

		// This window hosts each entity's seperate subbrowser window.
		// These subbrowser windows are accessed via a tab bar.
		CombatTrackerLib::ScopedWindow window{ entityBrowserWindowName, nullptr, ImGuiWindowFlags_NoTitleBar };

		// Tabs of the window host other windows for each entity (Monster and PlayerCharacter).
		// Creating a temporary stack allocated struct for this section is fine as we are just copying string_views.
		if (CombatTrackerLib::ScopedTabBar entityBrowserTabs{ "##EntityBrowserTabs" }) {
			if (CombatTrackerLib::ScopedTabItem tabItem{ getWindowName(MONSTER_BROWSER) }) {
				renderEntityBrowserWindow<MonsterCreationScene>(
					BrowserLabels{
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/monster/create"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/monster/edit"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/monster/delete"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/monster/emptyDetails"),
						"MonsterBrowser"
					},
					m_localeProviderPointer->getTextList("/scenes/encounterCreation/browser/monsterColumns"),
					m_monsterSource,
					m_monsterSearchQuery,
					m_selectedMonsterIndex
				);
			}
			if (CombatTrackerLib::ScopedTabItem tabItem{ getWindowName(PLAYER_CHARACTER_BROWSER) }) {
				renderEntityBrowserWindow<PlayerCharacterCreationScene>(
					BrowserLabels{
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/playerCharacter/create"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/playerCharacter/edit"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/playerCharacter/delete"),
						m_localeProviderPointer->getText("/scenes/encounterCreation/browser/playerCharacter/emptyDetails"),
						"PlayerCharacterBrowser"
					},
					m_localeProviderPointer->getTextList("/scenes/encounterCreation/browser/playerCharacterColumns"),
					m_playerCharacterSource,
					m_playerCharacterSearchQuery,
					m_selectedPlayerCharacterIndex
				);
			}
		}
	}
	void EncounterCreationScene::renderBrowserFilters(EntitySource& source, std::string& searchQuery, const char* label) {

		// Each tab passes its own label so that the widgets below don't collide in the ID stack.
		CombatTrackerLib::ScopedID scopedID{ label };
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(m_localeProviderPointer->getText("/scenes/encounterCreation/filters/source").data());
		ImGui::SameLine();
		if (ImGui::RadioButton(m_localeProviderPointer->getText("/scenes/encounterCreation/filters/all").data(), source == EntitySource::ALL)) {
			source = EntitySource::ALL;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton(m_localeProviderPointer->getText("/scenes/encounterCreation/filters/userCreated").data(), source == EntitySource::USER_CREATED)) {
			source = EntitySource::USER_CREATED;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton(m_localeProviderPointer->getText("/scenes/encounterCreation/filters/compendium").data(), source == EntitySource::COMPENDIUM)) {
			source = EntitySource::COMPENDIUM;
		}

		ImGui::SetNextItemWidth(-1.0f); // The search field spans the tab so it grows with the browser instead of sitting at ImGui's default item width.
		ImGui::InputTextWithHint("##Search", m_localeProviderPointer->getText("/scenes/encounterCreation/filters/searchHint").data(), &searchQuery);
	}

	// TODO: Finish this window when all monster and player character functionality is done.
	// This window is rendered as the "right" node of the dockspace.
	void EncounterCreationScene::renderEncounterManagerWindow() {

		using enum WindowType;

		CombatTrackerLib::ScopedWindow window{ getWindowName(ENCOUNTER_MANAGER) };
	}
}
